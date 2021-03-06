　このたびは、「WiZ（国際情報工科自動車大学校）C++ゲームフレームワーク（通称BaseCross）」GitHubサイト、にアクセスいただきありがとうございます。

　WiZ（国際情報工科自動車大学校）ホームページ
　http://www.wiz.ac.jp/index.html

　「BaseCross」は「クロスプラットフォーム」を目指すゲームのフレームワークです。
　これまで公開してきました「DxBase2015」「DxBase2016」の後継にあたります。
　2018年5月現在、「BaseCross」はDx11版につきましてはおおむねまとまってきました。

　また「BaseCross」にはモデルのデータ変換ツール「Fbx2BinVS2015」が含まれます。
　このツールをビルドするには「FBXSDK」および「MFC」が必要となります。
　（ゲームのサンプル等をビルドするだけであれば、これらは必要ありません）

　「BaseCross」のダウンロード方法は、この文書が表示されているページの、右側にあります、「DownloadZIP」のボタンをクリックして、パソコン等にダウンロードしてください。
　その際、zipを解凍する前にzipファイルを右クリックして「プロパティ」でセキュリティチェックを外してください。
　こうしないと、ソリューションが正常に読み込まれない場合があります。
　セキュリティチェックを外した後、zipを解凍してください。

　まずは「DirectX11」と「DirectX12」の「クロスプラットフォーム」の実装を進めてまいります。

　更新や追加情報はfacebookにて紹介いたします。
　もし更新情報などを知りたければ、facebookにて「YasushiYamanoi」を検索して、
https://wizframework.github.io/Documents/img/yy.png
　この似顔絵のアイコンを見つけましたら「友達依頼」を出してください。

　また、ドキュメントの部分をオンライン公開しました。

https://wizframework.github.io/Documents/

　ダウンロードする前にどんなものか見てみたい方など、ご利用ください。

　2017年夏現在メンテナンスは行ってませんが、過去のフレームワークは以下になります。
「DxBase2015」は
https://github.com/WiZFramework/DxBase2015
「DxBase2016」は
https://github.com/WiZFramework/DxBase2016
からダウンロードできます。
　また、「DxBase2016」で使用するモデルのデータ変換ツール「Fbx2Bin」は
https://github.com/WiZFramework/DxBaseTools
からダウンロードできます。
　使用方法等は各レポジトリを参照ください。
　メンテナンスしてないので、できれば「BaseCross」をご利用ください。


　また、動作を確認するには以下の環境が必要です
Visual Studio Community 2015（Community以外では動作確認してませんが、他は上位バージョンなので大丈夫と思われます）
＊VS2015をDX11でお使いになる場合は各サンプルの「BaseCrossDx11.sln」をお開きください。
＊VS2015をDX12でお使いになる場合は各サンプルの「BaseCrossDx12.sln」をお開きください。
Visual Studio Community 2017（フルバージョンサンプルのみの対応です。ただしFbx2BinVS2015は対応してません。）
＊VS2017をお使いになる場合は各サンプルの「BaseCrossDx11VS2017.sln」をお開きください。


＊セットアップ時に必ずVisualC++をセットアップしてください。
＊「DirectX11」版、動作にはWindows8.1以上が必要です。
＊「DirectX12」版、動作にはWindows10、Windows10SDK（10.0.10240.0以上）が必要です。
＊ライセンスは「MITライセンス」となります。改変など自由ですが、くれぐれもご自分の責任においてお使いください。



以下、主な更新履歴です（細かいのは、gitなのでレポジトリ内を参照ください）
2016年7月25日　初期アプリケーション公開
2016年7月28日　readme修正(動作環境)
2016年7月30日　シンプル、フル両バージョンテストサンプル公開
2016年8月02日　readme修正。ブログサイトのご紹介
2016年8月17日　readme修正。シンプルバージョンのサンプル作成開始
2016年9月04日　readme修正。シンプルバージョンのサンプル作成ひとまず完了
2016年9月04日　readme修正。フルバージョンのサンプル作成開始
2016年10月11日　readme修正。フルバージョン、コンポーネント含む基本的な機能実装
2017年01月05日　readme修正。フルバージョンドキュメント追加。Copyright年を2017に変更
2017年02月07日　readme修正。グラフィック充実のためのバージョンアップ開始
2017年02月28日　readme修正。2017春制作のための修正ひとまず完了
2017年03月16日　readme修正。2017春シェーダ修正ひとまず完了。
2017年08月05日　readme修正。ドキュメント全体的に見直し。
2017年11月14日　ドキュメントオンライン公開。
2017年12月11日　物理エンジン「Sony Computer Entertainment社のPhysics Effects（オープンソース版）」
	を実装しました。（BSDライセンスとなっています）。
2018年05月02日　VS2017対応（フルバージョンのみ）。

BaseCrossゲームフレームワーク日誌
開発日誌「-実験と実装と-」
http://gameproject.jp/
もありますが、しばらく更新が止まってます。



　　　　　　　　　　　　　　　　　　　　　2018年春　山ノ井　靖（WiZゲーム科非常勤講師：C++担当）
