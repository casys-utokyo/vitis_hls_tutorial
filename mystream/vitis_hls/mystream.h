#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef hls::axis<ap_int<32>, 0, 0, 0> StreamPix;

const int N = 512 * 512;
