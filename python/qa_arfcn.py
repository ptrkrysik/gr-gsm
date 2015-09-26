#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author Roman Khassraf <rkhassraf@gmail.com>
# @section LICENSE
# 
# Gr-gsm is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# Gr-gsm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with gr-gsm; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
# 

from gnuradio import gr, gr_unittest, blocks
import grgsm
import os
import sys


class qa_arfcn (gr_unittest.TestCase):
   
    def test_001_is_valid_arfcn(self):
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(259, 'GSM450'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(277, 'GSM450'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(293, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(258, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(294, 'GSM450'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(306, 'GSM480'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(323, 'GSM480'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(340, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(305, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(341, 'GSM480'))
                
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(128, 'GSM850'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(199, 'GSM850'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(251, 'GSM850'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(127, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(251, 'GSM480'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(1, 'P-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(63, 'P-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(124, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(0, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(125, 'P-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(975, 'E-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(999, 'E-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(1023, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(974, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(1024, 'E-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(955, 'R-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(989, 'R-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(1023, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(954, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(1024, 'R-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(512, 'DCS1800'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(732, 'DCS1800'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(885, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(511, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(886, 'DCS1800'))
        
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(512, 'PCS1900'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(691, 'PCS1900'))
        self.assertTrue(grgsm.arfcn.is_valid_arfcn(810, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(511, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_arfcn(811, 'PCS1900'))


    def test_002_is_valid_uplink(self):
        self.assertTrue(grgsm.arfcn.is_valid_uplink(450.6e6, 'GSM450'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(457.4e6, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(450.4e6, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(457.6e6, 'GSM450'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(479e6, 'GSM480'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(485.8e6, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(478.8e6, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(486e6, 'GSM480'))
                
        self.assertTrue(grgsm.arfcn.is_valid_uplink(824.2e6, 'GSM850'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(848.8e6, 'GSM850'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(824e6, 'GSM850'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(849e6, 'GSM850'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(890.2e6, 'P-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(914.8e6, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(890e6, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(915e6, 'P-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(880.2e6, 'E-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(889.8e6, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(880e6, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(890e6, 'E-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(876.2e6, 'R-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(889.8e6, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(876e6, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(890e6, 'R-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(1710.2e6, 'DCS1800'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(1784.8e6, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(1710e6, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(1785e6, 'DCS1800'))
        
        self.assertTrue(grgsm.arfcn.is_valid_uplink(1850.2e6, 'PCS1900'))
        self.assertTrue(grgsm.arfcn.is_valid_uplink(1909.8e6, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(1850e6, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_uplink(1910e6, 'PCS1900'))


    def test_003_is_valid_downlink(self):
        self.assertTrue(grgsm.arfcn.is_valid_downlink(460.6e6, 'GSM450'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(467.4e6, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(460.4e6, 'GSM450'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(467.6e6, 'GSM450'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(489e6, 'GSM480'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(495.8e6, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(488.8e6, 'GSM480'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(496e6, 'GSM480'))
                
        self.assertTrue(grgsm.arfcn.is_valid_downlink(869.2e6, 'GSM850'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(893.8e6, 'GSM850'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(869e6, 'GSM850'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(894e6, 'GSM850'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(935.2e6, 'P-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(959.8e6, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(935e6, 'P-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(960e6, 'P-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(925.2e6, 'E-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(934.8e6, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(925e6, 'E-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(935e6, 'E-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(921.2e6, 'R-GSM'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(934.8e6, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(921e6, 'R-GSM'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(935e6, 'R-GSM'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(1805.2e6, 'DCS1800'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(1879.8e6, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(1805e6, 'DCS1800'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(1880e6, 'DCS1800'))
        
        self.assertTrue(grgsm.arfcn.is_valid_downlink(1930.2e6, 'PCS1900'))
        self.assertTrue(grgsm.arfcn.is_valid_downlink(1989.8e6, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(1930e6, 'PCS1900'))
        self.assertFalse(grgsm.arfcn.is_valid_downlink(1990e6, 'PCS1900'))
        
    def test_004_arfcn2uplink(self):
        self.assertEqual(450.6e6, grgsm.arfcn.arfcn2uplink(259, 'GSM450'))
        self.assertEqual(457.4e6, grgsm.arfcn.arfcn2uplink(293, 'GSM450'))
        
        self.assertEqual(479e6, grgsm.arfcn.arfcn2uplink(306, 'GSM480'))
        self.assertEqual(485.8e6, grgsm.arfcn.arfcn2uplink(340, 'GSM480'))
        
        self.assertEqual(824.2e6, grgsm.arfcn.arfcn2uplink(128, 'GSM850'))
        self.assertEqual(848.8e6, grgsm.arfcn.arfcn2uplink(251, 'GSM850'))
        
        self.assertEqual(890.2e6, grgsm.arfcn.arfcn2uplink(1, 'P-GSM'))
        self.assertEqual(914.8e6, grgsm.arfcn.arfcn2uplink(124, 'P-GSM'))
        
        self.assertEqual(880.2e6, grgsm.arfcn.arfcn2uplink(975, 'E-GSM'))
        self.assertEqual(889.8e6, grgsm.arfcn.arfcn2uplink(1023, 'E-GSM'))
        
        self.assertEqual(876.2e6, grgsm.arfcn.arfcn2uplink(955, 'R-GSM'))
        self.assertEqual(889.8e6, grgsm.arfcn.arfcn2uplink(1023, 'R-GSM'))
        
        self.assertEqual(1710.2e6, grgsm.arfcn.arfcn2uplink(512, 'DCS1800'))
        self.assertEqual(1784.8e6, grgsm.arfcn.arfcn2uplink(885, 'DCS1800'))
        
        self.assertEqual(1850.2e6, grgsm.arfcn.arfcn2uplink(512, 'PCS1900'))
        self.assertEqual(1909.8e6, grgsm.arfcn.arfcn2uplink(810, 'PCS1900'))
        
    def test_005_arfcn2downlink(self):
        self.assertEqual(460.6e6, grgsm.arfcn.arfcn2downlink(259, 'GSM450'))
        self.assertEqual(467.4e6, grgsm.arfcn.arfcn2downlink(293, 'GSM450'))
        
        self.assertEqual(489e6, grgsm.arfcn.arfcn2downlink(306, 'GSM480'))
        self.assertEqual(495.8e6, grgsm.arfcn.arfcn2downlink(340, 'GSM480'))
        
        self.assertEqual(869.2e6, grgsm.arfcn.arfcn2downlink(128, 'GSM850'))
        self.assertEqual(893.8e6, grgsm.arfcn.arfcn2downlink(251, 'GSM850'))
        
        self.assertEqual(935.2e6, grgsm.arfcn.arfcn2downlink(1, 'P-GSM'))
        self.assertEqual(959.8e6, grgsm.arfcn.arfcn2downlink(124, 'P-GSM'))
        
        self.assertEqual(925.2e6, grgsm.arfcn.arfcn2downlink(975, 'E-GSM'))
        self.assertEqual(934.8e6, grgsm.arfcn.arfcn2downlink(1023, 'E-GSM'))
        
        self.assertEqual(921.2e6, grgsm.arfcn.arfcn2downlink(955, 'R-GSM'))
        self.assertEqual(934.8e6, grgsm.arfcn.arfcn2downlink(1023, 'R-GSM'))
        
        self.assertEqual(1805.2e6, grgsm.arfcn.arfcn2downlink(512, 'DCS1800'))
        self.assertEqual(1879.8e6, grgsm.arfcn.arfcn2downlink(885, 'DCS1800'))
        
        self.assertEqual(1930.2e6, grgsm.arfcn.arfcn2downlink(512, 'PCS1900'))
        self.assertEqual(1989.8e6, grgsm.arfcn.arfcn2downlink(810, 'PCS1900'))
        
    def test_006_uplink2arfcn(self):
        self.assertEqual(259, grgsm.arfcn.uplink2arfcn(450.6e6, 'GSM450'))
        self.assertEqual(293, grgsm.arfcn.uplink2arfcn(457.4e6, 'GSM450'))
        
        self.assertEqual(306, grgsm.arfcn.uplink2arfcn(479e6, 'GSM480'))
        self.assertEqual(340, grgsm.arfcn.uplink2arfcn(485.8e6, 'GSM480'))
        
        self.assertEqual(128, grgsm.arfcn.uplink2arfcn(824.2e6, 'GSM850'))
        self.assertEqual(251, grgsm.arfcn.uplink2arfcn(848.8e6, 'GSM850'))
        
        self.assertEqual(1, grgsm.arfcn.uplink2arfcn(890.2e6, 'P-GSM'))
        self.assertEqual(124, grgsm.arfcn.uplink2arfcn(914.8e6, 'P-GSM'))
        
        self.assertEqual(975, grgsm.arfcn.uplink2arfcn(880.2e6, 'E-GSM'))
        self.assertEqual(1023, grgsm.arfcn.uplink2arfcn(889.8e6, 'E-GSM'))
        
        self.assertEqual(955, grgsm.arfcn.uplink2arfcn(876.2e6, 'R-GSM'))
        self.assertEqual(1023, grgsm.arfcn.uplink2arfcn(889.8e6, 'R-GSM'))
        
        self.assertEqual(512, grgsm.arfcn.uplink2arfcn(1710.2e6, 'DCS1800'))
        self.assertEqual(885, grgsm.arfcn.uplink2arfcn(1784.8e6, 'DCS1800'))
        
        self.assertEqual(512, grgsm.arfcn.uplink2arfcn(1850.2e6, 'PCS1900'))
        self.assertEqual(810, grgsm.arfcn.uplink2arfcn(1909.8e6, 'PCS1900'))
        
    def test_007_downlink2arfcn(self):
        self.assertEqual(259, grgsm.arfcn.downlink2arfcn(460.6e6, 'GSM450'))
        self.assertEqual(293, grgsm.arfcn.downlink2arfcn(467.4e6, 'GSM450'))
        
        self.assertEqual(306, grgsm.arfcn.downlink2arfcn(489e6, 'GSM480'))
        self.assertEqual(340, grgsm.arfcn.downlink2arfcn(495.8e6, 'GSM480'))
        
        self.assertEqual(128, grgsm.arfcn.downlink2arfcn(869.2e6, 'GSM850'))
        self.assertEqual(251, grgsm.arfcn.downlink2arfcn(893.8e6, 'GSM850'))
        
        self.assertEqual(1, grgsm.arfcn.downlink2arfcn(935.2e6, 'P-GSM'))
        self.assertEqual(124, grgsm.arfcn.downlink2arfcn(959.8e6, 'P-GSM'))
        
        self.assertEqual(975, grgsm.arfcn.downlink2arfcn(925.2e6, 'E-GSM'))
        self.assertEqual(1023, grgsm.arfcn.downlink2arfcn(934.8e6, 'E-GSM'))
        
        self.assertEqual(955, grgsm.arfcn.downlink2arfcn(921.2e6, 'R-GSM'))
        self.assertEqual(1023, grgsm.arfcn.downlink2arfcn(934.8e6, 'R-GSM'))
        
        self.assertEqual(512, grgsm.arfcn.downlink2arfcn(1805.2e6, 'DCS1800'))
        self.assertEqual(885, grgsm.arfcn.downlink2arfcn(1879.8e6, 'DCS1800'))
        
        self.assertEqual(512, grgsm.arfcn.downlink2arfcn(1930.2e6, 'PCS1900'))
        self.assertEqual(810, grgsm.arfcn.downlink2arfcn(1989.8e6, 'PCS1900'))
        
    def test_008_firstarfcn(self):
        self.assertEqual(259, grgsm.arfcn.get_first_arfcn('GSM450'))
        self.assertEqual(306, grgsm.arfcn.get_first_arfcn('GSM480'))
        self.assertEqual(128, grgsm.arfcn.get_first_arfcn('GSM850'))
        self.assertEqual(1, grgsm.arfcn.get_first_arfcn('P-GSM'))
        self.assertEqual(975, grgsm.arfcn.get_first_arfcn('E-GSM'))
        self.assertEqual(955, grgsm.arfcn.get_first_arfcn('R-GSM'))
        self.assertEqual(512, grgsm.arfcn.get_first_arfcn('DCS1800'))
        self.assertEqual(512, grgsm.arfcn.get_first_arfcn('PCS1900'))

    def test_009_firstarfcn(self):
        self.assertEqual(293, grgsm.arfcn.get_last_arfcn('GSM450'))
        self.assertEqual(340, grgsm.arfcn.get_last_arfcn('GSM480'))
        self.assertEqual(251, grgsm.arfcn.get_last_arfcn('GSM850'))
        self.assertEqual(124, grgsm.arfcn.get_last_arfcn('P-GSM'))
        self.assertEqual(1023, grgsm.arfcn.get_last_arfcn('E-GSM'))
        self.assertEqual(1023, grgsm.arfcn.get_last_arfcn('R-GSM'))
        self.assertEqual(885, grgsm.arfcn.get_last_arfcn('DCS1800'))
        self.assertEqual(810, grgsm.arfcn.get_last_arfcn('PCS1900'))        
        
if __name__ == '__main__':
    gr_unittest.run(qa_arfcn, "qa_arfcn.xml")
 
