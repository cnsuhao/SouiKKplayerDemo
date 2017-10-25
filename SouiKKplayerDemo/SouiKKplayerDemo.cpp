// SouiKKplayerDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "UI.h"
#include "SouiKKplayerDemo.h"
#include "MainDlg.h"
#include "SVideoPlayer.h"
#ifdef _DEBUG
  #define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#else
   #define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
	using namespace SOUI;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    HRESULT hRes = CoInitialize(NULL);

    SOUI::CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory=NULL;
	SComMgr * pComMgr = new SComMgr;
	///GDI渲染器
	pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
	
	///图片解码器
	SOUI::CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
	pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
	pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
	
	
	SOUI::SApplication *theApp=new SApplication(pRenderFactory,hInstance);

     theApp->RegisterWindowClass<SVideoPlayer>();
	CAutoRefPtr<IResProvider>   pResProvider;
	CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
	if(pResProvider)
	{
	    HMODULE hSysResource=LoadLibrary(_T("kkplayerdemoUI.dll"));
		BOOL ret=pResProvider->Init((WPARAM)hSysResource,0);
	    theApp->AddResProvider(pResProvider);
	}

   
	//加载系统资源
	HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
	if(hSysResource){
		CAutoRefPtr<IResProvider> sysSesProvider;
		CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);

		sysSesProvider->Init((WPARAM)hSysResource,0);
		theApp->LoadSystemNamedResource(sysSesProvider);
	}

	
	
	CAutoRefPtr<ITranslatorMgr> trans;
	pComMgr->CreateTranslator((IObjRef**)&trans);
	if(trans)
	{
	        theApp->SetTranslator(trans);
            pugi::xml_document xmlLang;
            if(theApp->LoadXmlDocment(xmlLang,_T("lang_cn"),_T("translator"))){
                CAutoRefPtr<ITranslator> langCN;
                trans->CreateTranslator(&langCN);
                langCN->Load(&xmlLang.child(L"language"),1);//1=LD_XML
                trans->InstallTranslator(langCN);
            }
	}


	

	int 	nRet=0;
	
	SOUI::CMainDlg dlgMain;
	dlgMain.Create(NULL,WS_POPUP|WS_CLIPCHILDREN| WS_CLIPSIBLINGS,0,0,0,0,0);
	dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow(dlgMain.m_hWnd);
	dlgMain.ShowWindow(SW_SHOWNORMAL);
	nRet = theApp->Run(dlgMain.m_hWnd);/**/
}