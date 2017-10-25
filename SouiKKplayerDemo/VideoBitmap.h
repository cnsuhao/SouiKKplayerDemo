#ifndef VideoBitmap_H_
#define VideoBitmap_H_
#include "ui.h"
#include <interface/render-i.h>
namespace SOUI{
 class SVideoBitmap :public TObjRefImpl< SObjectImpl<IBitmap> > //TObjRefImpl<IBitmap>
{
    public:
        SVideoBitmap();
        ~SVideoBitmap();
        virtual HRESULT Init(int nWid,int nHei,const LPVOID pBits=NULL);
        virtual HRESULT Init(IImgFrame *pFrame);
        virtual HRESULT LoadFromFile(LPCTSTR pszFileName);
        virtual HRESULT LoadFromMemory(LPBYTE pBuf,size_t szLen);

        virtual UINT Width() const;
        virtual UINT Height() const;
        virtual SIZE Size() const;
        virtual LPVOID  LockPixelBits();
        virtual void    UnlockPixelBits(LPVOID);
        virtual const LPVOID GetPixelBits() const;
        virtual IRenderFactory * GetRenderFactory() const;
        HBITMAP  GetBitmap(){return m_hBmp;}
		LPVOID GetBuf(){
		   return m_pBmpBits;
		}
		int    GetBufLen(){
		    return m_bufLen;
		}
    protected:
		LPVOID m_pBmpBits;
        static HBITMAP CreateGDIBitmap(int nWid,int nHei,void ** ppBits);
        SIZE        m_sz;
		int         m_bufLen;
        HBITMAP     m_hBmp;     //标准的32位位图，和m_bitmap共享内存
    };
}
#endif