# matlab プログラムは mex プログラムと比べて遅い？

matlab プログラムは、条件分岐が多いような複雑な処理に向いていませんよね。
そこで、mex プログラムに置き換えて対応したら、100倍速くなった！なんてこと珍しくないと思います。

では、単純な処理についてはどうでしょうか？
早くなるんでしょうか？
今回は、それに挑戦してみました。

比較は、vector と vector の内積計算速度で行いました。
matlab のみで計算を行うのと、
mex で計算を行うのはどっちが速いか？
また、openmp や、SSE、AVX を使うとどうなるか？
やってみました。
（SSE：single×4並列、AVX：single×8並列）

### プログラムは以下です。
https://github.com/mucunwuxian/inner_product

実験条件は以下です。
・CPU：Intel(R) Core(TM) i7-4700MQ CPU @ 2.40GHz [core数:4、thread数:8]
・OS：Windows10 64bit
・MATLAB：2016a
・mex compiler：Microsoft Windows SDK 7.1 (C++)
・実験条件：vector長64,000,000 の内積演算 y = wTx を、100回実施する時間を測定

# 実験結果と考察

動かしてみた結果は以下です。

|condition         |time (sec)|cpu usage(%)|
|:-----------------|---------:|-----------:|
|matlab            |      6.00|         50%|
|mex               |     10.54|         12%|
|mex + openmp      |      6.03|        100%|
|mex + openmp + SSE|      3.32|        100%|
|mex + openmp + AVX|      4.03|        100%|

以上。
頑張れば、matlab を凌げる、という感じ。

面白いのは、openmp を使って並列演算を行っただけでは、
matlab 単体での演算速度と同程度とまでしか行かないところです。
CPU フルフルで使っているにも関わらず。
この理由は、matlab 単体計算に SIMD が使われているか、
Intel compiler による最適化が行われているか、という感じかと。

また、もう一つ特筆、
Microsoft Windows SDK 7.1 (C++) コンパイラによる、
SSE、AVX の最適化はちょっと甘い、と言えそうです。
コードに問題は無いと思うのですが、あまり高速化できてません。
これについては、また別途検証を行えれば、と思います。
（お金を払わないと、爆速実現難しいかなぁ、、、）

以上、今回はここまでとなります。
読んで下さった方、ありがとうございました。
