#include "mystream.h"

// top-level function
void mystream(hls::stream< StreamPix > &istrm, hls::stream< StreamPix > &ostrm) {
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE axis port=istrm
#pragma HLS INTERFACE axis port=ostrm

  for(int i = 0; i < N; i++) {
    // #pragma HLS PIPELINE II=1
    // read input
    StreamPix tmp;
    tmp = istrm.read();

    // computation
    StreamPix pix;
    //pix.data = 255 - tmp.data + 100; // NG
    pix.data = 255 - tmp.data;

    // AXI-Stream side-channel signals
    pix.keep = tmp.keep;
    pix.strb = tmp.strb;
    pix.last = tmp.last;
    if(i == N - 1) {
      pix.last = 1;
    }

    // write output
    ostrm.write(pix);
  }
}
