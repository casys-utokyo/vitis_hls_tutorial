#include <iostream>
#include "mystream.h"

void mystream(hls::stream< StreamPix > &istrm, hls::stream< StreamPix > &ostrm);

int main() {
  StreamPix pix;
  hls::stream< StreamPix > istrm("istrm");
  hls::stream< StreamPix > ostrm("ostrm");

  for(int step = 0; step < 10; step++){
    // input data
    int arr[N];
    for(int i = 0; i < N; i++){
      arr[i] = (i + step) % 256;
    }

    // to Stream
    for(int i = 0; i < N; i++){
      pix.data = arr[i];
      pix.last = 0;
      if(i == N - 1){
        pix.last = 1;
      }
      istrm << pix;
    }

    // simulate the target hardware function
    mystream(istrm, ostrm);

    // expected data
    int expected[N];
    for(int i = 0; i < N; i++){
      expected[i] = 255 - arr[i];
    }

    // compare
    StreamPix output;
    for(int i = 0; i < N; i++){
      output = ostrm.read();
      if(i == N - 1 && output.last != 1){
        std::cout << "FAIL: last mismatch" << std::endl;
        return 1;
      }
      if(output.data != expected[i]){
        std::cout << "FAIL: data mismatch: exected[" << i << "] = " << expected[i]
                  << ", but output = " << output.data << std::endl;
        return 1;
      }
    }

  }

  std::cout << "PASS" << std::endl;
  return 0;
}
