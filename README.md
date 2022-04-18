# libtorchviz
torchvizがPyTorchでしか使えないので、C++(libtorch)でも使えるように似たようなものを作ってみました。  
Windowsのスタティックライブラリとして作成してあります。DLL化の要望があれば行います。  
Windows用のモジュールしか用意していないません。Linux/macで利用したい人はソースからコンパイルしてください。  
Since torchviz can only be used with PyTorch, we created a similar feature so that it can also be used with C ++ (libtorch).

# Usage
[Graphviz](https://graphviz.org/download/)を利用しているのでインストールしてください。インストールするときにパスを通すようにして下さい。  
C++17の機能を利用しているので、コンパイラオプションでC++17を利用できるようにしてください。


