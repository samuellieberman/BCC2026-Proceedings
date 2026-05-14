/* Winelib does not have headers for the Windows C++ standard library. See
 * this StackOverflow page where someone else encountered this fact.
 * https://stackoverflow.com/questions/47946016/problems-compiling-c-programs-with-wineg-winelib
 * 
 * The declarations were all written by me, with the details copied from
 * the Microsoft GDI+ C++ API wrapper documentation here:
 * https://learn.microsoft.com/en-us/windows/win32/api/_gdiplus/
 * 
 * I eventually figured out that these headers contain definitions, not
 * just declarations. C++, amirite fellas? Implementations are adapted from
 * the MinGW64 unofficial Github mirror:
 * https://github.com/mingw-w64/mingw-w64/tree/master/mingw-w64-headers/include/gdiplus
 */
#if !defined(WINE_WINAPI) || !defined(__cplusplus)
#include <gdiplus_old.h>
#elif defined(WINE_WINAPI) && !defined(INTERCEPT_GDIPLUS_H)
#define INTERCEPT_GDIPLUS_H

#define Color Color_ignored
#define RectF RectF_ignored
#define SizeF SizeF_ignored
#define GpRectF GpRectF_ignored                                         // Uses RectF
#define GdipAddPathString GdipAddPathString_ignored                     // Uses RectF
#define GdipMeasureString GdipMeasureString_ignored                     // Uses RectF
#define GdipDrawString GdipDrawString_ignored                           // Uses RectF
#define GdipCreateLineBrushFromRect GdipCreateLineBrushFromRect_ignored // Uses RectF
extern "C" {
#include <gdiplus_old.h>
}
#undef Color
#undef RectF
#undef SizeF
#undef GpRectF
#undef GdipAddPathString
#undef GdipMeasureString
#undef GdipDrawString
#undef GdipCreateLineBrushFromRect

namespace Gdiplus {
// START OF REDECLARING *_ignored SYMBOLS
class Color;
class RectF;
typedef RectF GpRectF;

namespace DllExports {
extern "C" GpStatus WINGDIPAPI GdipAddPathString(
    GpPath*,
    GDIPCONST WCHAR*,
    INT,
    GDIPCONST GpFontFamily*,
    INT,
    REAL,
    GDIPCONST RectF*,
    GDIPCONST GpStringFormat*
);
extern "C" GpStatus WINGDIPAPI GdipMeasureString(
    GpGraphics*,
    GDIPCONST WCHAR*,
    INT,
    GDIPCONST GpFont*,
    GDIPCONST RectF*,
    GDIPCONST GpStringFormat*,
    RectF*,
    INT*,
    INT*
);
extern "C" GpStatus WINGDIPAPI GdipDrawString(
    GpGraphics*,
    GDIPCONST WCHAR*,
    INT,
    GDIPCONST GpFont*,
    GDIPCONST RectF*,
    GDIPCONST GpStringFormat*,
    GDIPCONST GpBrush*
);
extern "C" GpStatus WINGDIPAPI GdipCreateLineBrushFromRect(
    GDIPCONST GpRectF*,
    ARGB,
    ARGB,
    LinearGradientMode,
    GpWrapMode,
    GpLineGradient**
);
} // namespace DllExports
// END OF REDECLARING *_ignored SYMBOLS

using namespace DllExports;

// START OF REDEFINING *_ignored SYMBOLS
typedef struct SizeF {
    REAL Width;
    REAL Height;
    
    #ifdef __cplusplus
    SizeF(): Width(0.0f), Height(0.0f) {}
    SizeF(REAL width, REAL height): Width(width), Height(height) {}
    #endif /* __cplusplus */
} SizeF;
class Color {
private:
    ARGB Value;
public:
    static const ARGB Black = 0xFF000000;
    static ARGB MakeARGB(BYTE a, BYTE r, BYTE g, BYTE b) {
        return (ARGB) ((((DWORD) a) << 24) | (((DWORD) r) << 16) | (((DWORD) g) << 8) | ((DWORD) b));
    }
public:
    Color(): Value(Black) {}
    Color(ARGB argb): Value(argb) {}
    Color(BYTE r, BYTE g, BYTE b): Value(MakeARGB(0xFF, r, g, b)) {};
	Color(BYTE a, BYTE r, BYTE g, BYTE b): Value(MakeARGB(a, r, g, b)) {}
    COLORREF ToCOLORREF() const {
        return RGB((BYTE) Value, (BYTE) Value >> 8, (BYTE) Value >> 16);
    }
    VOID SetFromCOLORREF(COLORREF rgb) {
        BYTE r = (BYTE) rgb;
        BYTE g = (BYTE) (rgb >> 8);
        BYTE b = (BYTE) (rgb >> 16);
        Value = MakeARGB(0xFF, r, g, b);
    }
    ARGB GetValue() const {return Value;}
};
class RectF {
public:
    REAL X;
    REAL Y;
    REAL Width;
    REAL Height;
public:
    RectF(): X(0.0f), Y(0.0f), Width(0.0f), Height(0.0f) {}
    RectF(REAL x, REAL y, REAL width, REAL height): X(x), Y(y), Width(width), Height(height) {}
    RectF(const PointF & location, const SizeF & size):
        X(location.X),
        Y(location.Y),
        Width(size.Width),
        Height(size.Height)
    {}
};
// END OF REDEFINING *_ignored SYMBOLS

// Direct wrappers for flat API functions
Status GetImageEncodersSize(UINT *numEncoders, UINT *size);
//{
//    return GdipGetImageEncodersSize(numEncoders, size);
//}
Status GetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo *encoders);
//{
//    return GdipGetImageEncoders(numEncoders, size, encoders);
//}

// C++ wrapper classes for flat API functions
class Matrix;
class Brush;
class SolidBrush;
class LinearGradientBrush;
class Pen;
class FontCollection;
class FontFamily;
class Font;
class StringFormat;
class ImageAttributes;
class Image;
class Bitmap;
class GraphicsPath;
class Graphics;

class Matrix {
    friend class Graphics;
    friend class LinearGradientBrush;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpMatrix *nativeMatrix;
    mutable Status lastStatus;
public:
    Matrix(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy) {
        lastStatus = GdipCreateMatrix2(m11, m12, m21, m22, dx, dy, &nativeMatrix);
    }
    Matrix():  nativeMatrix(NULL), lastStatus(Ok) {
        GdipCreateMatrix(&nativeMatrix);
        lastStatus = GdipCreateMatrix(&nativeMatrix);
    }
    Status GetElements(REAL *m) {
        return updateStatus(GdipGetMatrixElements(nativeMatrix, m));
    }
    Status SetElements(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy) {
        return updateStatus(GdipSetMatrixElements(nativeMatrix, m11, m12, m21, m22, dx, dy));
    }
    Status Invert() {
        return updateStatus(GdipInvertMatrix(nativeMatrix));
    }
};

class Brush {
    friend class SolidBrush;
    friend class Graphics;
    friend class LinearGradientBrush;
    friend class Pen;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpBrush *nativeBrush;
    mutable Status lastStatus;
};
class SolidBrush : public Brush {
public:
    SolidBrush(const Color & color) {
        GpSolidFill *nativeSolidFill = NULL;
        lastStatus = GdipCreateSolidFill(color.GetValue(), &nativeSolidFill);
        nativeBrush = nativeSolidFill;
    }
    Status SetColor(const Color & color) {
        return updateStatus(GdipSetSolidFillColor((GpSolidFill*) nativeBrush, color.GetValue()));
    }
};
class LinearGradientBrush : public Brush {
public:
    // Unused but matches documentation.
    LinearGradientBrush(
        const RectF & rect,
        const Color & color1,
        const Color & color2,
        LinearGradientMode mode
        ) {
        GpLineGradient *nativeLineGradient = NULL;
        lastStatus = GdipCreateLineBrushFromRect(
            &rect,
            color1.GetValue(),
            color2.GetValue(),
            mode,
            WrapModeTile,
            &nativeLineGradient
        );
        nativeBrush = nativeLineGradient;
    }
    // Undocumented, but matches use.
    LinearGradientBrush(
        const RectF & rect,
        const Color & color1,
        const Color & color2,
        int mode
        ) {
        GpLineGradient *nativeLineGradient = NULL;
        lastStatus = GdipCreateLineBrushFromRect(
            &rect,
            color1.GetValue(),
            color2.GetValue(),
            static_cast<LinearGradientMode>(mode),
            WrapModeTile,
            &nativeLineGradient
        );
        nativeBrush = nativeLineGradient;
    }
    Status SetTransform(const Matrix *matrix) {
        return updateStatus(GdipSetTextureTransform(
            (GpTexture*) nativeBrush,
            matrix ? matrix->nativeMatrix : NULL
        ));
    }
    Status MultiplyTransform(const Matrix *matrix, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipMultiplyTextureTransform(
            (GpTexture*) nativeBrush,
            matrix ? matrix->nativeMatrix : NULL,
            order
        ));
    }
    Status TranslateTransform(REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipTranslateTextureTransform((GpTexture*) nativeBrush, dx, dy, order));
    }
    Status RotateTransform(REAL angle, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipRotateLineTransform((GpLineGradient*) nativeBrush, angle, order));
    }
    Status ScaleTransform(REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipScaleLineTransform((GpLineGradient*) nativeBrush, sx, sy, order));
    }
    Status SetWrapMode(WrapMode wrapMode) {
        return updateStatus(GdipSetLineWrapMode((GpLineGradient*) nativeBrush, wrapMode));
    }
    Status SetInterpolationColors(const Color *presetColors, const REAL *blendPositions, INT count) {
        if (!presetColors || count < 0) return lastStatus = InvalidParameter;
        
        ARGB *presetArgb = (ARGB*) GdipAlloc(count * sizeof(ARGB));
        if (!presetArgb) return lastStatus = OutOfMemory;
        
        for (INT i = 0; i < count; ++i) {
            presetArgb[i] = presetColors[i].GetValue();
        }
        
        Status status = updateStatus(GdipSetLinePresetBlend(
            (GpLineGradient*) nativeBrush,
            presetArgb,
            blendPositions,
            count
        ));
        GdipFree((void*) presetArgb);
        
        return status;
    }
};

class Pen {
    friend class Graphics;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpPen *nativePen;
    mutable Status lastStatus;
public:
    Pen(const Color & color, REAL width = 1.0): nativePen(NULL), lastStatus(Ok) {
        lastStatus = GdipCreatePen1(color.GetValue(), width, UnitWorld, &nativePen);
    }
    Status SetBrush(const Brush *brush) {
        return updateStatus(GdipSetPenBrushFill(nativePen, brush ? brush->nativeBrush : NULL));
    }
    Status SetColor(const Color & color) {
        return updateStatus(GdipSetPenColor(nativePen, color.GetValue()));
    }
    Status SetWidth(REAL width) {
        return updateStatus(GdipSetPenWidth(nativePen, width));
    }
    REAL GetWidth() const {
        REAL result = 1.0f;
        updateStatus(GdipGetPenWidth(nativePen, &result));
        return result;
    }
    Status GetDashPattern(REAL *dashArray, INT count) const {
        return updateStatus(GdipGetPenDashArray(nativePen, dashArray, count));
    }
    INT GetDashPatternCount(void)  const {
        INT result = 0;
        updateStatus(GdipGetPenDashCount(nativePen, &result));
        return result;
    }
    Status SetDashPattern(const REAL *dashArray, INT count) {
        return updateStatus(GdipSetPenDashArray(nativePen, dashArray, count));
    }
    Status SetDashStyle(DashStyle dashStyle) {
        return updateStatus(GdipSetPenDashStyle(nativePen, dashStyle));
    }
    DashStyle GetDashStyle() const {
        DashStyle result = DashStyleSolid;
        updateStatus(GdipGetPenDashStyle(nativePen, &result));
        return result;
    }
    Status SetLineCap(LineCap startCap, LineCap endCap, DashCap dashCap) {
        return updateStatus(GdipSetPenLineCap197819(nativePen, startCap, endCap, dashCap));
    }
    Status SetLineJoin(LineJoin lineJoin) {
        return updateStatus(GdipSetPenLineJoin(nativePen, lineJoin));
    }
};

class FontCollection {
    friend class FontFamily;
private:
    GpFontCollection *nativeFontCollection;
};
class FontFamily {
    friend class GraphicsPath;
    friend class Font;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpFontFamily *nativeFontFamily;
    mutable Status lastStatus;
public:
    FontFamily(const WCHAR *name, const FontCollection *fontCollection = NULL):
        nativeFontFamily(NULL),
        lastStatus(Ok)
    {
        lastStatus = GdipCreateFontFamilyFromName(
            name,
            fontCollection ? fontCollection->nativeFontCollection : NULL,
            &nativeFontFamily
        );
    }
};
class Font {
    friend class Graphics;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpFont *nativeFont;
    mutable Status lastStatus;
public:
    Font(const FontFamily *fontFamily, REAL emSize, INT style, Unit unit):
        nativeFont(NULL),
        lastStatus(Ok)
    {
        lastStatus = GdipCreateFont(
            fontFamily ? fontFamily->nativeFontFamily : NULL,
            emSize,
            style,
            unit,
            &nativeFont
        );
    }
};

class StringFormat {
    friend class GraphicsPath;
    friend class Graphics;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpStringFormat *nativeStringFormat;
    mutable Status lastStatus;
public:
    StringFormat(INT formatFlags = 0, LANGID language = LANG_NEUTRAL): nativeStringFormat(NULL), lastStatus(Ok) {
        GdipCreateStringFormat(formatFlags, language, &nativeStringFormat);
    }
public:
    Status SetAlignment(StringAlignment align) {
        return updateStatus(DllExports::GdipSetStringFormatAlign(nativeStringFormat, align));
    }
    Status SetTrimming(StringTrimming trimming) {
        return updateStatus(GdipSetStringFormatTrimming(nativeStringFormat, trimming));
    }
    Status SetLineAlignment(StringAlignment align) {
        return updateStatus(GdipSetStringFormatLineAlign(nativeStringFormat, align));
    }
};

class ImageAttributes {
    friend class Graphics;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpImageAttributes *nativeImageAttributes;
    mutable Status lastStatus;
public:
    Status SetColorMatrix(
        const ColorMatrix *colorMatrix,
        ColorMatrixFlags mode,
        ColorAdjustType type = ColorAdjustTypeDefault
    ) {
        return updateStatus(GdipSetImageAttributesColorMatrix(
            nativeImageAttributes,
            type,
            TRUE,
            colorMatrix,
            NULL,
            mode
        ));
    }
};
class Image {
    friend class Graphics;
    friend class Bitmap;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpImage *nativeImage;
    mutable Status lastStatus;
public:
    Image(GpImage *nativeImage, Status status): nativeImage(nativeImage), lastStatus(status) {}
    Image(IStream *stream, BOOL useEmbeddedColorManagement = FALSE):
        nativeImage(NULL),
        lastStatus(Ok)
    {
        if (useEmbeddedColorManagement) {
            lastStatus = GdipLoadImageFromStreamICM(stream, &nativeImage);
        } else {
            lastStatus = GdipLoadImageFromStream(stream, &nativeImage);
        }
    }
    Status SelectActiveFrame(const GUID *dimensionID, UINT frameIndex) {
        return updateStatus(GdipImageSelectActiveFrame(nativeImage, dimensionID, frameIndex));
    }
    UINT GetWidth() {
        UINT result = 0;
        updateStatus(GdipGetImageWidth(nativeImage, &result));
        return result;
    }
    UINT GetHeight() {
        UINT result = 0;
        updateStatus(GdipGetImageHeight(nativeImage, &result));
        return result;
    }
    Status SetPalette(const ColorPalette *palette) {
        return updateStatus(GdipSetImagePalette(nativeImage, palette));
    }
    PixelFormat GetPixelFormat() {
        PixelFormat result = (PixelFormat) 0;
        updateStatus(GdipGetImagePixelFormat(nativeImage, &result));
        return result;
    }
    INT GetPaletteSize() {
        INT result = 0;
        updateStatus(GdipGetImagePaletteSize(nativeImage, &result));
        return result;
    }
    Status GetPalette(ColorPalette *palette, INT size) {
        return updateStatus(GdipGetImagePalette(nativeImage, palette, size));
    }
    Status Save(
        IStream *stream,
        const CLSID *clsidEncoder,
        const EncoderParameters *encoderParams = NULL
    ) {
        return updateStatus(GdipSaveImageToStream(nativeImage, stream, clsidEncoder, encoderParams));
    }
};
class Bitmap : public Image {
public:
    static Bitmap * FromStream(IStream *stream, BOOL useEmbeddedColorManagement = FALSE) {
        return new Bitmap(stream, useEmbeddedColorManagement);
    }
    static Bitmap * FromHICON(HICON hicon) {
        return new Bitmap(hicon);
    }
public:
    Bitmap(HICON hicon): Image(NULL, Ok) {
        GpBitmap *nativeBitmap = NULL;
        lastStatus = GdipCreateBitmapFromHICON(hicon, &nativeBitmap);
        nativeImage = nativeBitmap;
    }
    Bitmap(IStream *stream, BOOL useEmbeddedColorManagement = FALSE): Image(NULL, Ok) {
        GpBitmap *nativeBitmap = NULL;
        if (useEmbeddedColorManagement) {
            lastStatus = GdipCreateBitmapFromStreamICM(stream, &nativeBitmap);
        } else {
            lastStatus = GdipCreateBitmapFromStream(stream, &nativeBitmap);
        }
        nativeImage = nativeBitmap;
    }
    Bitmap(INT width, INT height, PixelFormat format): Image(NULL, Ok) {
        GpBitmap *nativeBitmap = NULL;
        lastStatus = GdipCreateBitmapFromScan0(width, height, 0, format, NULL, &nativeBitmap);
        nativeImage = nativeBitmap;
    }
    Bitmap(INT width, INT height, INT stride, PixelFormat format, BYTE *scan0): Image(NULL, Ok) {
        GpBitmap *nativeBitmap = NULL;
        lastStatus = GdipCreateBitmapFromScan0(width, height, stride, format, scan0, &nativeBitmap);
        nativeImage = nativeBitmap;
    }
    Status LockBits(const Rect *rect, UINT flags, PixelFormat format, BitmapData *lockedBitmapData) {
        return updateStatus(GdipBitmapLockBits(
            (GpBitmap*) nativeImage,
            rect,
            flags,
            format,
            lockedBitmapData
        ));
    }
    Status UnlockBits(BitmapData *lockedBitmapData) {
        return updateStatus(GdipBitmapUnlockBits((GpBitmap*) nativeImage, lockedBitmapData));
    }
    Status GetFrameDimensionsList(GUID *dimensionIDs, UINT count) {
	    return updateStatus(GdipImageGetFrameDimensionsList(nativeImage, dimensionIDs, count));
    }
    UINT GetFrameCount(const GUID *dimensionID) {
	    UINT result = 0;
	    updateStatus(GdipImageGetFrameCount(nativeImage, dimensionID, &result));
	    return result;
	}
    UINT GetPropertyItemSize(PROPID propId) {
        UINT result = 0;
        updateStatus(GdipGetPropertyItemSize(nativeImage, propId, &result));
        return result;
    }
    Status GetPropertyItem(PROPID propId, UINT propSize, PropertyItem *buffer) {
        return updateStatus(GdipGetPropertyItem(nativeImage, propId, propSize, buffer));
    }
    Status GetHBITMAP(const Color & colorBackground, HBITMAP *hbmReturn) const
{
    return updateStatus(GdipCreateHBITMAPFromBitmap(
        (GpBitmap*) nativeImage,
        hbmReturn,
        colorBackground.GetValue()
    ));
}
    Status GetHICON(HICON *hicon) const {
        return updateStatus(GdipCreateHICONFromBitmap((GpBitmap*) nativeImage, hicon));
    };
};
class GraphicsPath {
    friend class Graphics;
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpPath *nativePath;
    mutable Status lastStatus;
public:
    GraphicsPath():  nativePath(NULL), lastStatus(Ok) {
        GdipCreatePath(FillModeAlternate, &nativePath);
    }
public:
    Status AddLine(REAL x1, REAL y1, REAL x2, REAL y2) {
        return updateStatus(GdipAddPathLine(nativePath, x1, y1, x2, y2));
    }
    Status CloseFigure() {
        return updateStatus(GdipClosePathFigure(nativePath));
    }
    Status AddArc(INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle) {
        return updateStatus(GdipAddPathArcI(nativePath, x, y, width, height, startAngle, sweepAngle));
    }
    Status AddEllipse(const RectF & rect) {
        return updateStatus(GdipAddPathEllipse(nativePath, rect.X, rect.Y, rect.Width, rect.Height));
    }
    Status AddLines(const PointF *points, INT count) {
        return updateStatus(GdipAddPathLine2(nativePath,points, count));
    }
    Status AddString(
        const WCHAR *string,
        INT length,
        const FontFamily *family,
        INT style,
        REAL emSize,
        const PointF & origin,
        const StringFormat *format
    ) {
        RectF layoutRect(origin, SizeF(0.0f, 0.0f));
        return updateStatus(GdipAddPathString(
            nativePath,
            string,
            length,
            family ? family->nativeFontFamily : NULL,
            style,
            emSize,
            &layoutRect,
            format ? format->nativeStringFormat : NULL
        ));
    }
};
class Graphics {
private:
    Status updateStatus(Status newStatus) const {
        if (newStatus != Ok) lastStatus = newStatus;
        return newStatus;
    }
    
    GpGraphics *nativeGraphics;
    mutable Status lastStatus;
public:
    Graphics(HDC hdc): nativeGraphics(NULL), lastStatus(Ok) {
        lastStatus = GdipCreateFromHDC(hdc, &nativeGraphics);
    }
    Graphics(Image *image) {
        lastStatus = GdipGetImageGraphicsContext(image ? image->nativeImage : NULL, &nativeGraphics);
    }
    Status ResetTransform() {
        return updateStatus(GdipResetWorldTransform(nativeGraphics));
    }
    Status TranslateTransform(REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipTranslateWorldTransform(nativeGraphics, dx, dy, order));
    }
    Status MultiplyTransform(const Matrix *matrix, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipMultiplyWorldTransform(
            nativeGraphics,
            matrix ? matrix->nativeMatrix : NULL,
            order
        ));
    }
    Status Clear(const Color & color) {
        return updateStatus(GdipGraphicsClear(nativeGraphics, color.GetValue()));
    }
    Status SetSmoothingMode(SmoothingMode smoothingMode) {
        return updateStatus(GdipSetSmoothingMode(nativeGraphics, smoothingMode));
    }
    Status SetTextRenderingHint(TextRenderingHint newMode) {
        return updateStatus(GdipSetTextRenderingHint(nativeGraphics, newMode));
    }
    Status GetTransform(Matrix *matrix) {
        return updateStatus(GdipGetWorldTransform(
            nativeGraphics,
            matrix ? matrix->nativeMatrix : NULL
        ));
    }
    Status DrawLine(const Pen *pen, REAL x1, REAL y1, REAL x2, REAL y2) {
        return updateStatus(GdipDrawLine(nativeGraphics, pen ? pen->nativePen : NULL, x1, y1, x2, y2));
    }
    Status DrawLines(const Pen *pen, const PointF *points, INT count) {
        return updateStatus(GdipDrawLines(nativeGraphics, pen ? pen->nativePen : NULL, points, count));
    }
    Status DrawArc(const Pen *pen, const RectF &rect, REAL startAngle, REAL sweepAngle) {
        return updateStatus(GdipDrawArc(
            nativeGraphics, pen ? pen->nativePen : NULL,
            rect.X,
            rect.Y,
            rect.Width,
            rect.Height,
            startAngle,
            sweepAngle
        ));
    }
    Status DrawBezier(
        const Pen *pen,
        REAL x1,
        REAL y1,
        REAL x2,
        REAL y2,
        REAL x3,
        REAL y3,
        REAL x4,
        REAL y4
    ) {
        return updateStatus(GdipDrawBezier(
            nativeGraphics, pen ? pen->nativePen : NULL,
            x1,
            y1,
            x2,
            y2,
            x3,
            y3,
            x4,
            y4
        ));
}
    Status DrawPath(const Pen *pen, const GraphicsPath *path) {
        return updateStatus(GdipDrawPath(
            nativeGraphics,
            pen ? pen->nativePen : NULL,
            path ? path->nativePath : NULL
        ));
    }
    Status FillPath(const Brush *brush, const GraphicsPath *path) {
        return updateStatus(GdipFillPath(
            nativeGraphics,
            brush ? brush->nativeBrush : NULL,
            path ? path->nativePath : NULL
        ));
    }
    Status MeasureString(
        const WCHAR *string,
        INT length,
        const Font *font,
        const RectF & layoutRect,
        RectF *boundingBox
    ) {
        return updateStatus(GdipMeasureString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            NULL,
            boundingBox,
            NULL,
            NULL
        ));
	}
    Status ScaleTransform(REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend) {
        return updateStatus(GdipScaleWorldTransform(nativeGraphics, sx, sy, order));
    }
    Status DrawString(
        const WCHAR *string,
        INT length,
        const Font *font,
        const PointF & origin,
        StringFormat *stringFormat,
        const Brush *brush
    ) {
        RectF layoutRect(origin.X, origin.Y, 0.0f, 0.0f);
        return updateStatus(GdipDrawString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            stringFormat ? stringFormat->nativeStringFormat : NULL,
            brush ? brush->nativeBrush : NULL
        ));
    }
    Status DrawString(
        const WCHAR *string,
        INT length,
        const Font *font,
        const RectF & layoutRect,
        StringFormat *stringFormat,
        const Brush *brush
    ) {
        return updateStatus(GdipDrawString(
            nativeGraphics,
            string,
            length,
            font ? font->nativeFont : NULL,
            &layoutRect,
            stringFormat ? stringFormat->nativeStringFormat : NULL,
            brush ? brush->nativeBrush : NULL
        ));
    }
    Status SetTransform(const Matrix *matrix) {
        return updateStatus(GdipSetWorldTransform(nativeGraphics, matrix ? matrix->nativeMatrix : NULL));
    }
    Status DrawImage(Image *image, INT x, INT y, INT width, INT height) {
        return updateStatus(GdipDrawImageRectI(
            nativeGraphics,
            image ? image->nativeImage : NULL,
            x,
            y,
            width,
            height
        ));
    }
    Status DrawImage(
        Image *image,
        RectF const & destRect,
        REAL srcx,
        REAL srcy,
        REAL srcwidth,
        REAL srcheight,
        Unit srcUnit = UnitPixel,
        const ImageAttributes *imageAttributes = NULL,
        DrawImageAbort callback = NULL,
        VOID *callbackData = NULL
    ) {
        return updateStatus(GdipDrawImageRectRect(
            nativeGraphics,
            image ? image->nativeImage : NULL,
            destRect.X,
            destRect.Y,
            destRect.Width,
            destRect.Height,
            srcx,
            srcy,
            srcwidth,
            srcheight,
            srcUnit,
            imageAttributes ? imageAttributes->nativeImageAttributes : NULL,
            callback,
            callbackData
        ));
    }
};

}; // namespace: Gdiplus


#endif