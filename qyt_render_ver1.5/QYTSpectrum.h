//
//  QYTSpectrum.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/26.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTSpectrum__
#define __qyt_render_ver1_5__QYTSpectrum__

#include "qytr.h"
#include "qyt_vector.h"

namespace QYT
{

    
    ///开始采样的波长
    static const int sampledLambdaStart = 400;
    
    ///结束采样的波长
    static const int sampledLambdaEnd = 700;
    
    ///采样数量
    static const int nSpectralSamples = 30;
    
    ///表示转换为RGB值之后，光谱代表的是反射光还是自发光
    enum class QYTSpectrumType { SPECTRUM_REFLECTANCE, SPECTRUM_ILLUMINANT };
    
    ///CIE标准的采样数目
    static const int nCIESamples = 471;
    
    ///CIE标准色度观察者光谱三刺激值（X）
    extern const QYTReal CIE_X[nCIESamples];
    
    ///CIE标准色度观察者光谱三刺激值（Y）
    extern const QYTReal CIE_Y[nCIESamples];
    
    ///CIE标准色度观察者光谱三刺激值（Z）
    extern const QYTReal CIE_Z[nCIESamples];
    
    ///CIE标准色度观察者光谱三刺激值采样波长
    extern const QYTReal CIE_lambda[nCIESamples];
    
    ///CIE标准色度观察者光谱三刺激值（Y）的积分，计算XYZ色度坐标时使用
    static const QYTReal CIE_Y_integral = 106.856895;
    
    /*需要将RGB刺激值转换为某种光谱表示时需要的数据。
     因为从光谱表示到RGB或者XYZ表示是绝对的，但是逆向
     转换却不是，一个RGB表示理论上对应无数种等效的SPD
     表示，因此若要进行逆向变换，最好遵从一些从实践中测
     的数据，这些数据计算的SPD比较符合真实情况。
     */
    static const int nRGB2SpectSamples = 32;
    extern const QYTReal RGB2SpectLambda[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectWhite[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectCyan[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectMagenta[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectYellow[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectRed[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectGreen[nRGB2SpectSamples];
    extern const QYTReal RGBRefl2SpectBlue[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectWhite[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectCyan[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectMagenta[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectYellow[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectRed[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectGreen[nRGB2SpectSamples];
    extern const QYTReal RGBIllum2SpectBlue[nRGB2SpectSamples];
    
    /**
     对于点采样的光谱表示，用来表示Xλ, Yλ，Zλ的SPD和XYZ匹配函数的卷积
     实际上就是对点采样值的加权求和，因此，我们还要确定SPD采样点所对应的
     波长。如果绕过这个问题来实现Spectrum类也是可能的。其中一个权益之计
     就是使用CRT彩色显像管中的标准红绿蓝三原色的光谱，虽然对于高质量的光
     谱计算而言这是不够的。高清晰电视标准已经有了一组关于这些RGB光谱的标
     准定义。
     该函数将XYZ颜色转换为RGB表示法，这个表示法依照某些上述标准进行创建。
     对于点采样的Spectrum表示而言，这是一个加权和。
     */
    inline void XYZToRGB(const QYTReal xyz[3], QYTReal rgb[3]) {
        rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
        rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
        rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
    }
    
    inline void RGBToXYZ(const QYTReal rgb[3], QYTReal xyz[3]) {
        xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
        xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
        xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
    }
    
    /**
     @class QYTCoefficientSpectrum
     使用特殊数目的采样点来表示光谱分布的采样
     */
    template<int nSamples>
    class QYTCoefficientSpectrum: public qyt_vector<QYTReal, nSamples>
    {
        typedef qyt_vector<QYTReal, nSamples> baseSpec;
        typedef QYTCoefficientSpectrum<nSamples> CoeffSpec;
    public:
        QYTCoefficientSpectrum(QYTReal v = 0.f):baseSpec(v){}
        QYTCoefficientSpectrum(const baseSpec& v):baseSpec(v){}
        
        QYTCoefficientSpectrum(const CoeffSpec& rhs):baseSpec(rhs){}
        
        QYTCoefficientSpectrum& operator=(const CoeffSpec& rhs)
        {
            if(this != &rhs)
                baseSpec::operator=(rhs);
            return *this;
        }
        
        bool isBlack() const
        {
            for (int i = 0; i < nSamples; ++i) {
                if(this->v[i] != 0.f)
                    return false;
            }
            return true;
        }
        
        bool hasNaNs() const {
            for (int i = 0; i < nSamples; ++i)
                if (isnan((*this)[i])) return true;
            return false;
        }
        
        template<int n>
        friend inline QYTCoefficientSpectrum<n> Pow(const QYTCoefficientSpectrum<n>& s, QYTReal e)
        {
            QYTCoefficientSpectrum<n> ret;
            for (int i = 0; i < n; ++i)
                ret.c[i] = powf(s[i], e);
            Assert(!ret.hasNaNs());
            return ret;
        }
        
        ///将s中的每一个值都取平方并返回
        friend CoeffSpec Sqrt(const CoeffSpec &s) {
            CoeffSpec ret;
            for (int i = 0; i < nSamples; ++i)
                ret[i] = sqrtf(s[i]);
            Assert(!ret.hasNaNs());
            return ret;
        }
        
        ///将每一个采样值都限制在low和high之间
        CoeffSpec clamp(QYTReal low = 0, QYTReal high = INFINITY) const {
            CoeffSpec ret;
            for (int i = 0; i < nSamples; ++i)
                ret[i] = QYTClamp(this->v[i], low, high);
            Assert(!ret.hasNaNs());
            return ret;
        }
        
        friend CoeffSpec exp(const CoeffSpec &s) {
            CoeffSpec ret;
            for (int i = 0; i < nSamples; ++i)
                ret[i] = expf(s.c[i]);
            Assert(!ret.hasNaNs());
            return ret;
        }
    };
    
    
    /**
     将采样的光谱数据按照波长进行排序。
     两个数组分辨含有采样波长及对应的值，该函数将这两个数组
     重新排序，使得波长从小到大排列，并且也使得对应的采样值
     一起被更新
     @param lambda  含有波长的数组
     @param vals    含有采样值的数组
     @param n       含有的采样的数量
     */
    void sortSpectrumSamples(QYTReal *lambda, QYTReal *vals, int n);
    
    /**
     判断采样数据是否已经按照波长进行排序
     两个数组分辨含有采样波长及对应的值，该函数判断波长数组已
     经按照从小到大的顺序排列好。
     @param lambda  含有波长的数组
     @param vals    含有采样值的数组
     @param n       含有的采样的数量
     @return    lambda是按照波长排序的则返回true，否则返回false
     */
    bool spectrumSamplesSorted(const QYTReal *lambda, const QYTReal *vals, int n);
    
    /**
     *计算lambdaStart和lambdaEnd之间的辐射通量的平均值，这个函数
     返回一个波长区间内SPD分布的平均值，用于对SPD进行重采样。也可以
     单纯地用作某个范围内的平均值计算，例如也可以对CIE标准匹配函数进
     行重采样。
     */
    QYTReal averageSpectrumSamples(const QYTReal *lambda, const QYTReal *vals,
                                int n, QYTReal lambdaStart, QYTReal lambdaEnd);
    
    QYTReal interpolateSpectrumSamples(const QYTReal *lambda, const QYTReal *vals,
                                            int n, float l);
    
    class QYTRGBSpectrum;
    
    /**
     @class QYTSampledSpectrum
     它存储在固定波长上采样的一组采样值。
     这个表示方法对于变化相对平缓SPD还是很好的，也可以做为评判其它表示方法的基准。
     如果该方法采用大量的采样值（例如按1nm的间隔采样），就可以作为一种参考实现（当
     然这要花费大量的计算资源）。这种方法的基本操作（例如SPD的加运算和乘运算）还是
     很高效的，其复杂性跟采样值的个数呈线性关系。
     
     这种表示的缺点是不能适应那些含有尖峰的SPD（比如日光灯的SPD）。如果采样值错过了
     尖峰，就会产生非常不准确的图像。
     */
    class QYTSampledSpectrum:public QYTCoefficientSpectrum<nSpectralSamples>
    {
    private:
        typedef QYTSampledSpectrum SampledSpectrum;
        
        ///对CIE标准色度观察者光谱三刺激值进行重采样而得到的新值
        ///每一个中含有nSpectralSamples个数据
        static SampledSpectrum X, Y, Z;
        
        /*
         需要进行逆向变换时，会用到下面的重采样数据。
         */
        static SampledSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
        static SampledSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
        static SampledSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
        static SampledSpectrum rgbRefl2SpectBlue;
        static SampledSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
        static SampledSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
        static SampledSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
        static SampledSpectrum rgbIllum2SpectBlue;
        
    public:
        
        QYTSampledSpectrum(QYTReal v = 0.f):QYTCoefficientSpectrum<nSpectralSamples>(v){}
        QYTSampledSpectrum(const QYTCoefficientSpectrum<nSpectralSamples>& v):QYTCoefficientSpectrum<nSpectralSamples>(v){}
        
        QYTSampledSpectrum(const qyt_vec<nSpectralSamples>& v):QYTCoefficientSpectrum<nSpectralSamples>(v){}
        
        QYTSampledSpectrum(const QYTRGBSpectrum& r, QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE);
        
        /**
         对原始SPD进行重采样，将原始SPD重新按波长排序，并且压缩到nSpectralSamples
         指定的采样数量。
         @param lambda  原始SPD的对应波长
         @param v       原始SPD的值
         @param n       原始SPD的采样数（不是nSpectralSamples）
         @return        根据原始SPD重新采样为nSpectralSamples个采样数的新Spectrum
         */
        static SampledSpectrum FromSampled(const QYTReal* lambda,
                                           const QYTReal* v,
                                           int n)
        {
            //首先要对采样进行排序
            if(!spectrumSamplesSorted(lambda, v, n))
            {
                std::vector<QYTReal> slambda(&lambda[0], &lambda[n]);
                std::vector<QYTReal> sv(&v[0], &v[n]);
                sortSpectrumSamples(&slambda[0], &sv[0], n);
                
                //递归
                return FromSampled(&slambda[0], &sv[0], n);
            }
            SampledSpectrum r;
            for (int i = 0; i < nSpectralSamples; ++i)
            {
                //在采样波长范围（400~700nm）之间取30个采样波长，这些采样波长是插值得出的
                QYTReal lambda0 = QYTLerp(QYTReal(i) / QYTReal(nSpectralSamples),
                                     sampledLambdaStart, sampledLambdaEnd);
                
                QYTReal lambda1 = QYTLerp(QYTReal(i+1) / QYTReal(nSpectralSamples),
                                     sampledLambdaStart, sampledLambdaEnd);
                
                //计算两个采样波长间隔间的辐射通量平均值
                r[i] = averageSpectrumSamples(lambda, v, n, lambda0, lambda1);
            }
            return r;
        }
        
        /**
         初始化所有计算必须的原始数据
         */
        static void Init()
        {
            //初始化CIE标准色度观察者光谱三刺激值
            for (int i = 0; i < nSpectralSamples; ++i)
            {
                QYTReal wl0 = QYTLerp(QYTReal(i) / QYTReal(nSpectralSamples),
                                      sampledLambdaStart, sampledLambdaEnd);
                QYTReal wl1 = QYTLerp(QYTReal(i+1) / QYTReal(nSpectralSamples),
                                      sampledLambdaStart, sampledLambdaEnd);
                X[i] = averageSpectrumSamples(CIE_lambda, CIE_X, nCIESamples,
                                              wl0, wl1);
                Y[i] = averageSpectrumSamples(CIE_lambda, CIE_Y, nCIESamples,
                                              wl0, wl1);
                Z[i] = averageSpectrumSamples(CIE_lambda, CIE_Z, nCIESamples,
                                              wl0, wl1);
            }
            
            // Compute RGB to spectrum functions for _SampledSpectrum_
            for (int i = 0; i < nSpectralSamples; ++i) {
                float wl0 = QYTLerp(float(i) / float(nSpectralSamples),
                                 sampledLambdaStart, sampledLambdaEnd);
                float wl1 = QYTLerp(float(i+1) / float(nSpectralSamples),
                                 sampledLambdaStart, sampledLambdaEnd);
                rgbRefl2SpectWhite[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectWhite,
                                                                 nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectCyan[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectCyan,
                                                                nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectMagenta[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectMagenta,
                                                                   nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectYellow[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectYellow,
                                                                  nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectRed[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectRed,
                                                               nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectGreen[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectGreen,
                                                                 nRGB2SpectSamples, wl0, wl1);
                rgbRefl2SpectBlue[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectBlue,
                                                                nRGB2SpectSamples, wl0, wl1);
                
                rgbIllum2SpectWhite[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectWhite,
                                                                  nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectCyan[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectCyan,
                                                                 nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectMagenta[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectMagenta,
                                                                    nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectYellow[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectYellow,
                                                                   nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectRed[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectRed,
                                                                nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectGreen[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectGreen,
                                                                  nRGB2SpectSamples, wl0, wl1);
                rgbIllum2SpectBlue[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectBlue,
                                                                 nRGB2SpectSamples, wl0, wl1);
            }
            
        }
        
        /**
         将光谱表示转化为XYZ（色品）坐标。
         */
        void toXYZ(QYTReal xyz[3]) const {
            xyz[0] = xyz[1] = xyz[2] = 0.f;
            for (int i = 0; i < nSpectralSamples; ++i) {
                xyz[0] += X[i] * this->v[i];
                xyz[1] += Y[i] * this->v[i];
                xyz[2] += Z[i] * this->v[i];
            }

            xyz[0] /= CIE_Y_integral;
            xyz[1] /= CIE_Y_integral;
            xyz[2] /= CIE_Y_integral;
        }
        
        
        /**
         将光谱表示转化为RGB刺激值。
         @attention 不是一般说的0~255表示的24位RGB色！
         */
        void toRGB(QYTReal rgb[3]) const {
            QYTReal xyz[3];
            toXYZ(xyz);
            XYZToRGB(xyz, rgb);
        }
        
        ///由一个RGB表示法创建一个光谱表示对象
        ///@attention 是RGB刺激值，不是一般说的0~255表示的24位RGB色！
        static SampledSpectrum FromRGB(const QYTReal rgb[3],
                                       QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE);
        
        ///由XYZ色品坐标创建光谱表示对象
        static SampledSpectrum FromXYZ(const QYTReal xyz[3],
                                       QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE) {
            QYTReal rgb[3];
            XYZToRGB(xyz, rgb);
            return FromRGB(rgb, type);
        }
    };
    
    /**
     @class QYTRGBSpectrum
     和QYTSampledSpectrum类似，不过仅仅保存3个采样，并且自动初始化为RGB刺激值的形式。
     */
    class QYTRGBSpectrum:public QYTCoefficientSpectrum<3>
    {
        typedef QYTRGBSpectrum RGBSpectrum;
    public:
        QYTRGBSpectrum(QYTReal v = 0.f):QYTCoefficientSpectrum<3>(v){}
        QYTRGBSpectrum(const qyt_vec<3>& v):QYTCoefficientSpectrum<3>(v){}
        QYTRGBSpectrum(QYTReal r, QYTReal g, QYTReal b)
        {
            this->v[0] = r;
            this->v[1] = g;
            this->v[2] = b;
        }
        
        QYTRGBSpectrum(const QYTCoefficientSpectrum<3>& v):QYTCoefficientSpectrum<3>(v){}
        QYTRGBSpectrum(const QYTRGBSpectrum&s, QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE)
        {
            *this = s;
        }
        
        static RGBSpectrum FromRGB(const QYTReal rgb[3],
                                   QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE) {
            RGBSpectrum s;
            s[0] = rgb[0];
            s[1] = rgb[1];
            s[2] = rgb[2];
            Assert(!s.hasNaNs());
            return s;
        }
        
        void toRGB(QYTReal *rgb) const {
            rgb[0] = this->v[0];
            rgb[1] = this->v[1];
            rgb[2] = this->v[2];
        }
        
        const RGBSpectrum &toRGBSpectrum() const {
            return *this;
        }
        
        void toXYZ(float xyz[3]) const {
            RGBToXYZ(this->v, xyz);
        }
        
        static RGBSpectrum FromXYZ(const QYTReal xyz[3],
                                   QYTSpectrumType type = QYTSpectrumType::SPECTRUM_REFLECTANCE) {
            RGBSpectrum r;
            XYZToRGB(xyz, r);
            return r;
        }
        
        static RGBSpectrum FromSampled(const QYTReal *lambda, const QYTReal *v,
                                       int n) {
            // Sort samples if unordered, use sorted for returned spectrum
            if (!spectrumSamplesSorted(lambda, v, n)) {
                std::vector<float> slambda(&lambda[0], &lambda[n]);
                std::vector<float> sv(&v[0], &v[n]);
                sortSpectrumSamples(&slambda[0], &sv[0], n);
                return FromSampled(&slambda[0], &sv[0], n);
            }
            float xyz[3] = { 0, 0, 0 };
            float yint = 0.f;
            for (int i = 0; i < nCIESamples; ++i) {
                yint += CIE_Y[i];
                float val = interpolateSpectrumSamples(lambda, v, n,
                                                       CIE_lambda[i]);
                xyz[0] += val * CIE_X[i];
                xyz[1] += val * CIE_Y[i];
                xyz[2] += val * CIE_Z[i];
            }
            xyz[0] /= yint;
            xyz[1] /= yint;
            xyz[2] /= yint;
            return FromXYZ(xyz);
        }
    };

    
    typedef QYTRGBSpectrum QYTSpectrum;
    
    inline QYTSpectrum Lerp(QYTReal t, const QYTSpectrum &s1, const QYTSpectrum &s2) {

        return QYTSpectrum((QYTCoefficientSpectrum<3>)((1.f - t) * s1 + t * s2));
    }
    
}

#endif /* defined(__qyt_render_ver1_5__QYTSpectrum__) */
