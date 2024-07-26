#include "matvec.h"

inline void load_array(dtype *global, dtype *local, unsigned int goffset, unsigned int loffset,
                 unsigned int num_words){
  l_load: for(unsigned int i=0; i<num_words; i++){
    #pragma HLS loop_tripcount min=4 max=(1024 * 16) avg=1024
    #pragma HLS dependence variable=global type=inter false
    #pragma HLS dependence variable=global type=intra false
    #pragma HLS dependence variable=local type=inter false
    #pragma HLS dependence variable=local type=intra false
    local[loffset + i] = global[goffset + i];
  }
}

inline void store_array(dtype *global, dtype *local, unsigned int goffset, unsigned int loffset,
                 unsigned int num_words){
  l_store: for(unsigned int i=0; i<num_words; i++){
    #pragma HLS loop_tripcount min=4 max=(1024 * 16) avg=1024
    #pragma HLS dependence variable=global type=inter false
    #pragma HLS dependence variable=global type=intra false
    #pragma HLS dependence variable=local type=inter false
    #pragma HLS dependence variable=local type=intra false
    global[goffset + i] = local[loffset + i];
  }
}

// top-level function
void matvec(dtype *mat, dtype *vec, dtype *out,
            unsigned int vec_len, unsigned int out_len) {
#pragma HLS INTERFACE mode=s_axilite port=return bundle=control
#pragma HLS INTERFACE mode=s_axilite port=vec_len bundle=control
#pragma HLS INTERFACE mode=s_axilite port=out_len bundle=control
#pragma HLS INTERFACE mode=m_axi port=mat depth=16384 offset=slave bundle=gmem_in0
#pragma HLS INTERFACE mode=s_axilite port=mat bundle=control
#pragma HLS INTERFACE mode=m_axi port=vec depth=16384 offset=slave bundle=gmem_in0
#pragma HLS INTERFACE mode=s_axilite port=vec bundle=control
#pragma HLS INTERFACE mode=m_axi port=out depth=16384 offset=slave bundle=gmem_out0
#pragma HLS INTERFACE mode=s_axilite port=out bundle=control

  dtype mat_buf [MAX_LEN];
  dtype vec_buf [MAX_LEN];
  dtype out_buf [MAX_LEN];
#pragma HLS ARRAY_PARTITION variable=mat_buf type=cyclic factor=4
#pragma HLS ARRAY_PARTITION variable=vec_buf type=cyclic factor=4
//#pragma HLS ARRAY_PARTITION variable=out_buf type=cyclic factor=4

  unsigned int num_comp = out_len;
  if(num_comp >= MAX_LEN) {
    num_comp = MAX_LEN;
  }

  unsigned int num_words = vec_len;
  if(num_words >= MAX_LEN) {
    num_words = MAX_LEN;
  }

  // load vec from external memory
  load_array(vec, vec_buf, 0, 0, num_words);

  l_comp: for(unsigned int step=0; step<num_comp; step++){
    #pragma HLS loop_tripcount min=4 max=(1024 * 16) avg=1024
    //#pragma HLS DATAFLOW
    // load a row of mat from external memory
    load_array(mat, mat_buf, step * num_words, 0, num_words);

    dtype sum = 0;

    // inner-product
    l_innerproduct: for(unsigned int i=0; i<num_words; i++){
      #pragma HLS loop_tripcount min=4 max=(1024 * 16) avg=1024
      #pragma HLS PIPELINE II=1
      #pragma HLS UNROLL factor=4
      sum += mat_buf[i] * vec_buf[i];
    }

    // store result
    out_buf[step] = sum;
  }

  // store result to external memory
  store_array(out, out_buf, 0, 0, num_comp);
}
