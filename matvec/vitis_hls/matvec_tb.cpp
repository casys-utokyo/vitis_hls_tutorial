#include <iostream>
#include "matvec.h"

void matvec(dtype *mat, dtype *vec, dtype *out, unsigned int vec_len, unsigned int out_len);

int main() {
  // input data
  const unsigned int vec_len = 128;
  const unsigned int out_len = 128;

  dtype mat [vec_len * out_len];
  dtype vec [vec_len];
  dtype out [out_len];

  for(unsigned int i=0; i<out_len; i++){
    for(unsigned int j=0; j<vec_len; j++){
      mat[vec_len * i + j] = (i * j + 1) % 10 - 5;
      //mat[vec_len * i + j] = 1;
    }
  }

  for(unsigned int i=0; i<vec_len; i++){
    vec[i] = (i % 10) - 5;
  }

  // simulate the target hardware function
  matvec(mat, vec, out, vec_len, out_len);

  // expected data
  dtype expected[out_len];

  for(unsigned int i=0; i<out_len; i++){
    dtype sum = 0;
    for(unsigned int j=0; j<vec_len; j++){
       sum += mat[vec_len * i + j] * vec[j];
    }
    expected[i] = sum;
  }

  // compare
  for(unsigned int i=0; i<out_len; i++){
    if(out[i] != expected[i]){
      std::cout << "FAIL: data mismatch: expected[" << i << "] = "
                << expected[i]
                << ", but out[" << i << "] = " << out[i]
                << std::endl;
      return 1;
    }
  }

  std::cout << "PASS" << std::endl;
  return 0;
}
