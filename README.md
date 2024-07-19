コンピューティングアーキテクチャ Vitis HLSチュートリアル
====================

はじめに
--------------------

この資料はVitis HLSを用いたハードウェア設計への入門者向けチュートリアルである。
プラグマ等の詳細は[ug1399 Vitis高位合成ユーザーガイド](https://docs.amd.com/r/ja-JP/ug1399-vitis-hls)を参照して頂きたい。

開発環境
--------------------

ACRiルームのサーバーを用いた演習を想定している。

- Vitis 2022.1

Vitis HLSを用いたハードウェア設計フロー
--------------------

### ハードウェアの設計

### Cシミュレーションの実行

### C合成

### C/RTL協調シミュレーション


プラグマを用いた計算の並列化
--------------------

- pipeline
- unroll
- dataflow


AXI StreamとDMACを用いたストリーム型データ入出力
--------------------

専用のデータ型 `hls::stream` を用いることでストリーム型入出力を明示的に構成できる。

### メリット

- HLSハードウェア側でデータの入出力を管理する必要がない
- 他のAXI Stream入出力と直結することで、タスクパイプラインを実現可能

### デメリット

- CPUソフトウェアによるDMAの制御が必要
- 複雑なメモリアクセスパターンの実装が困難


M_AXI (AXI Master) を用いた柔軟なデータ入出力
--------------------

配列中の任意のデータにアクセス可能なアクセラレータを実現する場合、


## シングルアクセス

## バーストアクセス

- forループによるバーストアクセスの表現
- memcpyによるバーストアクセスの表現（非推奨）
(- cacheプラグマによるキャッシュの利用)
