﻿Imports System.Threading
Imports System.Runtime.InteropServices.Marshal
Imports System.Drawing.Imaging

Namespace ImageProcessing
    Public Class GrayScale : Inherits ComImgProc
        Public Sub New(_bitmap As Bitmap)
            MyBase.New(_bitmap)
        End Sub

        Protected Overrides Sub Finalize()
            MyBase.Finalize()
        End Sub

        Public Overrides Function GoImgProc(_token As CancellationToken) As Boolean
            Dim bRst As Boolean = True

            Dim nWidthSize As Integer = MyBase.m_bitmap.Width
            Dim nHeightSize As Integer = MyBase.m_bitmap.Height

            Dim bitmapData As BitmapData = MyBase.m_bitmap.LockBits(New Rectangle(0, 0, nWidthSize, nHeightSize), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb)

            Dim nIdxWidth As Integer
            Dim nIdxHeight As Integer

            For nIdxHeight = 0 To nHeightSize - 1 Step 1
                For nIdxWidth = 0 To nWidthSize - 1 Step 1
                    If (_token.IsCancellationRequested) Then
                        bRst = False
                        Exit For
                    End If

                    Dim pAdr As IntPtr = bitmapData.Scan0
                    Dim nPos As Integer = nIdxHeight * bitmapData.Stride + nIdxWidth * 4
                    Dim nPixelB As Integer = ReadByte(pAdr, nPos + ComInfo.Pixel.B)
                    Dim nPixelG As Integer = ReadByte(pAdr, nPos + ComInfo.Pixel.G)
                    Dim nPixelR As Integer = ReadByte(pAdr, nPos + ComInfo.Pixel.R)

                    Dim nGrayScale As Integer = (nPixelB + nPixelG + nPixelR) / 3
                    WriteByte(pAdr, nPos + ComInfo.Pixel.B, CByte(nGrayScale))
                    WriteByte(pAdr, nPos + ComInfo.Pixel.G, CByte(nGrayScale))
                    WriteByte(pAdr, nPos + ComInfo.Pixel.R, CByte(nGrayScale))
                Next
            Next
            MyBase.m_bitmap.UnlockBits(bitmapData)

            Return bRst
        End Function
    End Class
End Namespace