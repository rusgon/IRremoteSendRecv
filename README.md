IRremoteSendRecv
================

In the IrRemote library, both the IRrecv and IRSender use the same timer. That means the IRrecv and IRSender can't work concurrently due to timer resource competition. IRremoteSendRecv library allows transmitting and receiving data simultaneously