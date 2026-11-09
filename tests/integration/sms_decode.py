# SPDX-License-Identifier: GPL-3.0-or-later
"""Extract short messages from decoded SDCCH messages.

A small parser for the tests, it is not a general SMS library. The input are the
payloads (23 bytes, LAPDm frames) that gr-gsm's control channel decoder produces:

    LAPDm I frames (SAPI 3)  ->  reassembly of the segments  ->  CP-DATA (3GPP TS 24.011)
    ->  RP-DATA / RP-ACK  ->  TPDU (3GPP TS 23.040)  ->  text in the GSM 7 bit alphabet
"""

import struct

# 3GPP TS 23.038, the default alphabet and its extension table
GSM7 = ("@£$¥èéùìòÇ\nØø\rÅåΔ_ΦΓΛΩΠΨΣΘΞ\x1bÆæßÉ !\"#¤%&'()*+,-./0123456789:;<=>?"
        "¡ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÑÜ§¿abcdefghijklmnopqrstuvwxyzäöñüà")
GSM7_EXTENSION = {0x0A: "\f", 0x14: "^", 0x28: "{", 0x29: "}", 0x2F: "\\",
                  0x3C: "[", 0x3D: "~", 0x3E: "]", 0x40: "|", 0x65: "€"}


def gsmtap_fields(pdu):
    """Return (frame number, uplink, timeslot, subslot, payload) of a GSMTAP packet."""
    header = pdu[1] * 4
    arfcn = struct.unpack_from("!H", pdu, 4)[0]
    frame = struct.unpack_from("!I", pdu, 8)[0]
    return frame, bool(arfcn & 0x4000), pdu[3], pdu[14], bytes(pdu[header:])


def gsm7_decode(data, septets):
    bits = int.from_bytes(data, "little")
    text, escape = [], False
    for index in range(septets):
        code = (bits >> (7 * index)) & 0x7F
        if escape:
            text.append(GSM7_EXTENSION.get(code, " "))
            escape = False
        elif code == 0x1B:
            escape = True
        else:
            text.append(GSM7[code])
    return "".join(text)


def digits(data, count):
    out = "".join(f"{byte & 0xF:X}{byte >> 4:X}" for byte in data)
    return out[:count].replace("A", "*").replace("B", "#")


def i_frames(messages):
    """LAPDm information frames: direction, subslot, SAPI, N(S), 'more' bit and the data."""
    frames = []
    for frame, uplink, _timeslot, subslot, payload in messages:
        address, control = payload[0], payload[1]
        if control & 1:  # supervisory or unnumbered frame
            continue
        length_indicator = payload[2]
        length = length_indicator >> 2
        frames.append({
            "frame": frame, "uplink": uplink, "subslot": subslot,
            "sapi": (address >> 2) & 7, "ns": (control >> 1) & 7,
            "more": bool(length_indicator & 2),
            "data": bytes(payload[3:3 + length]),
        })
    return frames


def l3_messages(frames):
    """Join the segments of every (direction, subslot, SAPI) into complete layer 3 messages.

    Returns (messages, unfinished): the messages are (direction, subslot, SAPI, first frame,
    number of segments, bytes), unfinished is the list of the keys that ended in the middle of a
    message.
    """
    open_segments, messages = {}, []
    for frame in sorted(frames, key=lambda f: f["frame"]):
        key = (frame["uplink"], frame["subslot"], frame["sapi"])
        segments = open_segments.setdefault(key, [])
        segments.append(frame)
        if not frame["more"]:
            messages.append((*key, segments[0]["frame"], len(segments), b"".join(s["data"] for s in segments)))
            open_segments[key] = []
    return messages, [key for key, segments in open_segments.items() if segments]


def _address(data, offset):
    length = data[offset]
    return data[offset + 1:offset + 1 + length], offset + 1 + length


def parse_tpdu(tpdu):
    kind = tpdu[0] & 3
    if kind == 1:  # SMS-SUBMIT
        count = tpdu[2]
        size = (count + 1) // 2
        index = 4 + size
        validity = {0: 0, 1: 7, 2: 1, 3: 7}[(tpdu[0] >> 3) & 3]
        index += 2 + validity  # PID, DCS and the validity period
        septets = tpdu[index]
        return {"type": "SMS-SUBMIT", "reference": tpdu[1], "address": digits(tpdu[4:4 + size], count),
                "dcs": tpdu[index - 1 - validity], "length": septets, "data": tpdu[index + 1:]}
    if kind == 0:  # SMS-DELIVER
        count = tpdu[1]
        size = (count + 1) // 2
        index = 3 + size + 2 + 7  # PID, DCS, time stamp
        return {"type": "SMS-DELIVER", "address": digits(tpdu[3:3 + size], count),
                "dcs": tpdu[3 + size + 1], "length": tpdu[index], "data": tpdu[index + 1:]}
    return {"type": f"TPDU {kind}"}


def parse_cp(message):
    """Parse a CP layer message, None if the message is not an SMS."""
    if len(message) < 2 or (message[0] & 0x0F) != 9:
        return None
    if message[1] != 0x01:
        return {"type": {0x04: "CP-ACK", 0x10: "CP-ERROR"}.get(message[1], f"CP {message[1]:#x}")}
    rp = message[3:3 + message[2]]
    kind = rp[0] & 7
    result = {"type": "CP-DATA", "rp_type": {0: "RP-DATA", 1: "RP-DATA", 2: "RP-ACK", 3: "RP-ACK", 4: "RP-ERROR",
                                              5: "RP-ERROR", 6: "RP-SMMA"}.get(kind, f"RP {kind}"),
              "rp_reference": rp[1]}
    if kind in (0, 1):
        _, index = _address(rp, 2)
        _, index = _address(rp, index)
        result["tpdu"] = parse_tpdu(rp[index + 1:index + 1 + rp[index]])
    return result


def short_messages(messages):
    """All SMS related layer 3 messages of the decoded messages, parsed.

    Returns a list of (direction, first frame, parsed message). The text of an SMS in the
    default alphabet is added as `text`.
    """
    l3, unfinished = l3_messages(i_frames(messages))
    result = []
    for uplink, _subslot, _sapi, frame, _segments, data in l3:
        parsed = parse_cp(data)
        if parsed is None:
            continue
        tpdu = parsed.get("tpdu")
        if tpdu and "data" in tpdu and (tpdu["dcs"] & 0x0C) == 0:
            tpdu["text"] = gsm7_decode(tpdu["data"], tpdu["length"])
        result.append(("UL" if uplink else "DL", frame, parsed))
    return result, unfinished
