/******************************************************************************/
//	DirectX9ライブラリ
//		テキスト
/******************************************************************************/

//----------------------------------------------------------------------------//
//	インクルード
//----------------------------------------------------------------------------//
#include	"./Dx9Lib.h"

//----------------------------------------------------------------------------//
//	グローバル変数
//----------------------------------------------------------------------------//
ST_TextInfo			TXT[TXT_MAX];



/******************************************************************************/
//	名前	：	コンストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibText::CDx9LibText()
{
	m_lpFont = NULL;
	m_lpTxtSprite = NULL;
}

/******************************************************************************/
//	名前	：	デストラクタ
//	説明	：	なし
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
CDx9LibText::~CDx9LibText()
{
	this->Release();
}

/******************************************************************************/
//	名前	：	テキストデバイス初期化
//	説明	：	デバイスの初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::InitDevice( void )
{
	//	フォントの設定
	D3DXFONT_DESC	FontDesc;
	FontDesc.Height = FONT_SIZE;
	FontDesc.Width = 0;
	FontDesc.Weight = FW_NORMAL;
	FontDesc.MipLevels = 0;
	FontDesc.Italic = FALSE;
	FontDesc.CharSet = DEFAULT_CHARSET;
	FontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	FontDesc.Quality = DEFAULT_QUALITY;
	FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s( FontDesc.FaceName, LF_FACESIZE, "ＭＳ ゴシック" );

	// フォントデバイスの作成
	D3DXCreateFontIndirect(
		g_lpD3DDev,
		&FontDesc,
		&m_lpFont );

	//	スプライト作成
	D3DXCreateSprite( g_lpD3DDev, &m_lpTxtSprite );
}

/******************************************************************************/
//	名前	：	テキスト全初期化
//	説明	：	テキスト構造体の全初期化を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::InitAll( void )
{
	for( long i=0; i<TXT_MAX; i++ )
	{
		this->Init( i );
	}
}

/******************************************************************************/
//	名前	：	テキスト初期化
//	説明	：	テキスト構造体の初期化を行う
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::Init( long TextNo )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	//	フラグ
	TXT[ TextNo ].Disp = OFF;
	TXT[ TextNo ].Use = OFF;

	//	座標系
	TXT[ TextNo ].PosX = 0;
	TXT[ TextNo ].PosY = 0;
	TXT[ TextNo ].CenterX = 0;
	TXT[ TextNo ].CenterY = 0;

	//	表示サイズ
	TXT[ TextNo ].Width = g_WindowWidth;
	TXT[ TextNo ].Height = g_WindowHeight;

	//	各色成分の初期化
	TXT[ TextNo ].A = 255;
	TXT[ TextNo ].R = 255;
	TXT[ TextNo ].G = 255;
	TXT[ TextNo ].B = 255;

	TXT[ TextNo ].Time = -2;
	TXT[ TextNo ].Atr = ATR_NONE;
	TXT[ TextNo ].Speed = 10;
	TXT[ TextNo ].Count = 0;
	TXT[ TextNo ].Step = -1;
	TXT[ TextNo ].State = 0;

	//	格納用文字列
	ZeroMemory( &TXT[ TextNo ].Str, sizeof(TXT[ TextNo ].Str) );
}

/******************************************************************************/
//	名前	：	テキスト全表示
//	説明	：	テキストの表示を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::DispAll( void )
{
	m_lpTxtSprite->Begin(NULL);

	for( long i=0; i<TXT_MAX; i++ )
	{
		if( TXT[i].Use == OFF || TXT[i].Disp == OFF )
		{
			continue;
		}

		g_LibText.Disp( i );

		if( TXT[i].Time > -2 )
		{
			TXT[i].Time --;
		}
		if( TXT[i].Time == 0 )
		{
			TXT[i].Time = 0;
			TXT[i].Disp = OFF;
		}
	}

	m_lpTxtSprite->End();
}

/******************************************************************************/
//	名前	：	テキストセット
//	説明	：	テキストをセットする
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]double	PosX		表示座標X
//				[IN]double	PosY		表示座標Y
//				[IN]UChar	A			アルファ
//				[IN]UChar	R			赤成分
//				[IN]UChar	G			緑成分
//				[IN]UChar	B			青成分
//				[IN]char	*str		登録する文字列
//				[IN]UChar	Atr			アトリビュート
//				[IN]long	Speed		表示速度
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::Set(long TextNo,
						 double PosX, double PosY,
						 UChar A, UChar R, UChar G, UChar B,
						 char *Str,
						 UChar Atr,
						 long Speed )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Use= ON;
	TXT[ TextNo ].Disp = ON;

	strcpy_s( TXT[ TextNo ].Str, MAX_PATH, Str );

	TXT[ TextNo ].PosX = PosX;
	TXT[ TextNo ].PosY = PosY;

	TXT[ TextNo ].A = A;
	TXT[ TextNo ].R = R;
	TXT[ TextNo ].G = G;
	TXT[ TextNo ].B = B;

	TXT[ TextNo ].Time = -2;
	TXT[ TextNo ].Atr = Atr;
	TXT[ TextNo ].Speed = Speed;
	TXT[ TextNo ].Count = 0;
	if( Atr == ATR_ONCE )
	{
		TXT[ TextNo ].Step = 0;
	}
}

/******************************************************************************/
//	名前	：	表示変更
//	説明	：	テキストの表示を変更する
//	戻り値	：	なし
//	引数	：	[IN]UChar	Flag	表示フラグ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::ChangeDisp( UChar Flag )
{
	for( long i=0; i<TXT_MAX; i++ )
	{
		if( TXT[ i ].Use == ON )
		{
			TXT[ i ].Disp = Flag;
		}
	}
}

void	CDx9LibText::ChangeDisp( long TextNo, UChar Flag )
{
	TXT[ TextNo ].Disp = Flag;
}

/******************************************************************************/
//	名前	：	カラーセット
//	説明	：	テキストの色変更
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]UChar	A			アルファ
//				[IN]UChar	R			赤成分
//				[IN]UChar	G			緑成分
//				[IN]UChar	B			青成分
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::SetColor( long TextNo, UChar A, UChar R, UChar G, UChar B )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].A = A;
	TXT[ TextNo ].R = R;
	TXT[ TextNo ].G = G;
	TXT[ TextNo ].B = B;
}

/******************************************************************************/
//	名前	：	表示座標設定
//	説明	：	表示座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]double	PosX		表示座標X
//				[IN]double	PosY		表示座標Y
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::SetPos( long TextNo, double PosX, double PosY )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].PosX = PosX;
	TXT[ TextNo ].PosY = PosY;
}

/******************************************************************************/
//	名前	：	中心座標設定
//	説明	：	中心座標を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::SetCenter( long TextNo )
{
	char	*pt;
	UChar	uc;
	long	len,max;

	if( TextNo >= TXT_MAX )
	{
		return;
	}

	//	準備
	len = (long)strlen( TXT[ TextNo ].Str );
	pt = (char*)TXT[ TextNo ].Str;
	max = 0;

	for( int i=0; i<len; i++ )
	{
		uc = *pt;
		if( (uc<128) || (uc>=160) )
		{
			//	半角
			max ++;
			pt++;
		}
		else
		{
			//	全角
			max += 2;
			pt += 2;
		}
	}

	TXT[ TextNo ].CenterX = ( len * FONT_SIZE ) / 4;
	TXT[ TextNo ].CenterY = FONT_SIZE / 2;
}

/******************************************************************************/
//	名前	：	表示領域設定
//	説明	：	表示領域を設定する
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]short	Witdh		横サイズ
//				[IN]short	Height		縦サイズ
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::SetRect( long TextNo, long Width, long Height )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Width = Width;
	TXT[ TextNo ].Height = Height;
}

/******************************************************************************/
//	名前	：	データコピー
//	説明	：	データをコピーする
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]long	CopyNo		コピー元テキスト番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::CopyData( long TextNo, long CopyNo )
{
	if( TextNo >= TXT_MAX || CopyNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ] = TXT[ CopyNo ];
}

/******************************************************************************/
//	名前	：	タイマセット
//	説明	：	タイマを設定する
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//				[IN]long	Time		表示フレーム数
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::SetTimer( long TextNo, long Time )
{
	if( TextNo >= TXT_MAX )
	{
		return;
	}

	TXT[ TextNo ].Time = Time;
}

/******************************************************************************/
//	名前	：	オブジェクト解放
//	説明	：	オブジェクトの解放を行う
//	戻り値	：	なし
//	引数	：	なし
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::Release()
{
	SAFE_RELEASE(m_lpTxtSprite);
	SAFE_RELEASE(m_lpFont);
}

/******************************************************************************/
//	名前	：	テキスト表示
//	説明	：	テキストの表示を行う
//	戻り値	：	なし
//	引数	：	[IN]long	TextNo		テキスト番号
//	備考	：	なし
/******************************************************************************/
void	CDx9LibText::Disp( long TextNo )
{
	long	len,format;
	RECT	rect;

	//	デフォルト色設定
	D3DCOLOR color
		= D3DCOLOR_ARGB(TXT[TextNo].A,TXT[TextNo].R,TXT[TextNo].G,TXT[TextNo].B);

	len = (long)strlen( TXT[TextNo].Str );
	format = DT_WORDBREAK;
	rect.left = (long)( TXT[TextNo ].PosX - (TXT[TextNo].CenterX) ); 
	rect.right = TXT[TextNo].Width;
	rect.top = (long)( TXT[TextNo ].PosY - (TXT[TextNo].CenterY) );
	rect.bottom = TXT[TextNo].Height;

	// クリッピング
	if( (rect.right < 1) || (rect.left >= TXT[TextNo].Width)
	|| (rect.bottom < 1) || (rect.top >= TXT[TextNo].Height) )
	{
		return;
	}

	// 左端補正
	if( rect.left < 0 )
	{
		rect.left = 0;
		format |= DT_RIGHT;	// 右揃え
	}

	// 上端補正
	if( rect.top < 0 )
	{
		rect.top = 0;
		format |= DT_BOTTOM | DT_SINGLELINE;	// 下揃え、１行
	}

	//	文字列から”[#”を検索する
	char	*pTok = strstr( TXT[TextNo].Str, "[#" );
	if( pTok == NULL )
	{
		if( TXT[TextNo].Atr == ATR_ONCE )
		{
			long	Length = (long)strlen(TXT[TextNo].Str);
			if( Length > TXT[TextNo].Step && !TXT[TextNo].State )
			{
				TXT[TextNo].Count ++;
				if( TXT[TextNo].Count > TXT[TextNo].Speed )
				{
					TXT[TextNo].Count = 0;
					TXT[TextNo].Step ++;
					if( TXT[TextNo].Str[TXT[TextNo].Step] < 128 || TXT[TextNo].Str[TXT[TextNo].Step]>= 160 )
					{
						//	半角なので何もしない
					}
					else
					{
						TXT[TextNo].Step ++;
					}
				}
			}
			else
			{
				TXT[TextNo].State = ON;
			}
		}
		else
		{
			TXT[TextNo].State = ON;
		}

		// デバイス上に、整形した ANSI テキストを描画する
		m_lpFont->DrawText(NULL, TXT[TextNo].Str,
			//-1,
			TXT[TextNo].Step,
			&rect, format, color );
		//m_lpFont->DrawText(lpTxtSprite, TXT[No].Str, -1, &rect, format, color );
	}
	else
	{
		string	TargetStr = TXT[TextNo].Str;
		//	文字列分割
		vector<string>	SplitStr = SplitText(TargetStr, "[");
		vector<string>::iterator	Ite;
		long Pos = (long)TXT[TextNo].PosX;

		for( Ite=SplitStr.begin(); Ite!=SplitStr.end(); Ite++ )
		{
			//	再度文字列分割
			vector<string>	SplitStr2 = SplitText( *(Ite), "]" );
			vector<string>::iterator	Ite2;
			string	DispStr;
			for( Ite2=SplitStr2.begin(); Ite2!=SplitStr2.end(); Ite2++ )
			{
				if( "#RED" == *(Ite2) )
				{
					color = COLOR_RED;
				}
				else if( "#GREEN" == *(Ite2) )
				{
					color = COLOR_GREEN;
				}
				else if( "#BLUE" == *(Ite2) )
				{
					color = COLOR_BLUE;
				}
				else if( "#DEF" == *(Ite2) )
				{
					color
						= D3DCOLOR_ARGB( TXT[TextNo].A, TXT[TextNo].R, TXT[TextNo].G, TXT[TextNo].B );
				}
				else if( Ite2->find_first_of("#") != Ite2->npos )
				{
					//	「#00000000」～「#FFFFFFFF」を発見
					color = ToDec( Ite2->c_str() );
				}
				else
				{
					DispStr = *(Ite2);
				}
			}

			m_lpFont->DrawText(m_lpTxtSprite, DispStr.c_str(), -1, &rect, format, color );
			//m_lpFont->DrawText(NULL, DispStr.c_str(), -1, &rect, format, color );
			rect.left += (long)DispStr.length() * FONT_SIZE /2;
		}

		TXT[TextNo].State = 1;
	}
}

