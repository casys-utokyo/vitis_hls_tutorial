コンピューティングアーキテクチャVitis HLSチュートリアル
====================

はじめに
--------------------

この資料はVitis HLSを用いたハードウェア設計への入門者向けチュートリアルである。
プラグマ等の詳細は[ug1399 Vitis高位合成ユーザーガイド](https://docs.amd.com/r/ja-JP/ug1399-vitis-hls)を参照して頂きたい。

環境
--------------------

### 開発環境

ACRiルームのサーバーを用いた演習を想定しているが、ローカルの環境でも差し支えない。

- Ubuntu 20.04 または Ubuntu 22.04
- Vitis 2022.1

### FPGA環境

- FPGAボード: Kria KV260 Vision AI Starter Kit
- FPGA上のOS: Ubuntu 22.04
    - Ubuntu公式のSDカードイメージを用いる https://ubuntu.com/download/amd
    - ddコマンド等でSDカードのイメージを書き込む
    - 公式ページのフローに従って "Booting your Starter Kit section" まで進める https://www.amd.com/en/products/system-on-modules/kria/k26/kv260-vision-starter-kit/getting-started-ubuntu/booting-your-starter-kit.html
- FPGA制御フレームワーク: PYNQ (main branch commit: 21bf5e18a6dba20babe99108ac697d2cdc49645f)
    - KV260上のターミナルで、以下のコマンドを実行し、PYNQをインストールする（要ネットワーク）
      ```
      git clone https://github.com/Xilinx/Kria-PYNQ.git
      cd Kria-PYNQ
      sudo bash install.sh -b KV260
      ```

構成
--------------------

- README.md
- mystream: AXI StreamとDMACを用いたストリーム型データ入出力
    - vitis_hls
        - mystream.cpp: ハードウェア本体
        - mystream_tb.cpp: Cシミュレーション
        - mystream.h: ハードウェア本体およびCシミュレーションの共通ヘッダー
    - pynq
        - mystream.ipynb: Jupyter Lab上の制御ソフトウェア
        - mystream.bit: 合成済みビットファイル
        - mystream.hwh: 合成済みビットファイルの構成ファイル
- matvec: AXI Masterを用いた自立的なデータ入出力
    - vitis_hls
        - matvec.cpp: ハードウェア本体
        - matvec_tb.cpp: Cシミュレーション
        - matvec.h: ハードウェア本体およびCシミュレーションの共通ヘッダー
        - raytracing.png: テスト用画像データ
    - pynq
        - matvec.ipynb: Jupyter Lab上の制御ソフトウェア
        - matvec.bit: 合成済みビットファイル
        - matvec.hwh: 合成済みビットファイルの構成ファイル


開発フロー
--------------------

### Vitis HLS

- ハードウェアの設計
    - `vitis_hls` 下の `foo.cpp`, `foo.h` を利用
- Cシミュレーションの実行
    - `vitis_hls` 下の `foo_tb.cpp` を利用
- C合成
- （オプション）C/RTL協調シミュレーション

### Vivado

- 自作IPコアのライブラリへの取り込み
- ブロックデザインによるIPコアの統合
- 論理合成
- 配置配線
- ビットストリーム合成
- ビットストリームと構成ファイルのエクスポート
    - `foo.bit` と `foo.hwh` を取得

### PYNQ (Jupyter Lab)

- PYNQライブラリを用いた制御ソフトウェア開発
     - `pynq` 下の `foo.ipynb` を利用


プロジェクト
--------------------

### mystream: AXI StreamとDMACを用いたストリーム型データ入出力

グレースケール画像の入力を想定した、色を反転するストリーム計算回路である。
専用のデータ型 `hls::stream` を用いることでストリーム型入出力を明示的に実現している。

ストリーム型の入出力を用いることで、HLSハードウェア側でデータの入出力を管理する必要がなくなり、ハードウェア型の制御が簡素化される。
また、他のAXI Stream入出力と直結することで、タスクパイプラインを実現可能することも可能である。

一方、CPU上のソフトウェアによるDMAの制御が必要であり、連続するアドレスへのアクセス以外の複雑なメモリアクセスパターンの対応は容易ではない。


### matvec: AXI Masterを用いた自立的なデータ入出力

行列・ベクトル積の計算回路である。
入出力はAXI Master (m_axi) 形式のインターフェースで実現されており、forループによる連続アクセスの記述からバースト転送が推論されている。
引数の配列の先頭アドレスは、AXI-Lite Slaveインターフェースで実現される制御レジスタを介して、ソフトウェアから渡すことができる。

ベクトル側をBlock RAMで実現されるオンチップメモリに保持し、再利用している。
`pipeline`, `unroll`, `array_partiion`プラグマを用いて並列化およびオンチップメモリの最適化をしている。

AXI Streamを用いる方法とは異なり、ハードウェア側が自立的に外部メモリアクセスすることができるため、データの再利用や、複雑なメモリアクセスパターンに柔軟に対応することができる。
