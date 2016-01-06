from gnuradio import gr
from distutils.version import LooseVersion as version

#class created to solve incompatibility of reginstration of message inputs
#that was introduced in gnuradio 3.7.9

class hier_block(gr.hier_block2):
    def message_port_register_hier_in(self, port_id):
        if version(gr.version()) >= version('3.7.9'):
            super(hier_block, self).message_port_register_hier_in(port_id)
        else:
            super(hier_block, self).message_port_register_hier_out(port_id)

    def message_port_register_hier_out(self, port_id):
        if version(gr.version()) >= version('3.7.9'):
            super(hier_block, self).message_port_register_hier_out(port_id)
        else:
            super(hier_block, self).message_port_register_hier_in(port_id)

