#include "alfe/main.h"
#include "alfe/cga.h"
#include "alfe/config_file.h"

class MandelWindow : public RootWindow
{
public:
    void setOutput(CGAOutput* output) { _output = output; }
    void setConfig(ConfigFile* configFile, File configPath)
    {
        _configFile = configFile;
        _sequencer.setROM(
            File(configFile->get<String>("cgaROM"), configPath.parent()));

        _output->setConnector(1);  // new composite,  0 for RGBI
        _output->setScanlineProfile(0);    // rectangle
        _output->setHorizontalProfile(0);  // rectangle
        _output->setScanlineWidth(1);
        _output->setScanlineBleeding(2);   // symmetrical
        _output->setHorizontalBleeding(2); // symmetrical
        _output->setZoom(2);
        _output->setHorizontalRollOff(0);
        _output->setHorizontalLobes(4);
        _output->setVerticalRollOff(0);
        _output->setVerticalLobes(4);
        _output->setSubPixelSeparation(1);
        _output->setPhosphor(0);           // colour
        _output->setMask(0);
        _output->setMaskSize(0);
        _output->setAspectRatio(5.0/6.0);
        _output->setOverscan(0.1);
        _output->setCombFilter(0);         // no filter
        _output->setHue(0);
        _output->setSaturation(100);
        _output->setContrast(100);
        _output->setBrightness(0);
        _output->setShowClipping(false);
        _output->setChromaBandwidth(1);
        _output->setLumaBandwidth(1);
        _output->setRollOff(0);
        _output->setLobes(1.5);
        _output->setPhase(1);

        _regs = -CGAData::registerLogCharactersPerBank;
        _cgaBytes.allocate(0x4000 + _regs);
        _vram = &_cgaBytes[_regs];
        _vram[CGAData::registerLogCharactersPerBank] = 12;
        _vram[CGAData::registerScanlinesRepeat] = 1;
        _vram[CGAData::registerHorizontalTotalHigh] = 0;
        _vram[CGAData::registerHorizontalDisplayedHigh] = 0;
        _vram[CGAData::registerHorizontalSyncPositionHigh] = 0;
        _vram[CGAData::registerVerticalTotalHigh] = 0;
        _vram[CGAData::registerVerticalDisplayedHigh] = 0;
        _vram[CGAData::registerVerticalSyncPositionHigh] = 0;
        _vram[CGAData::registerMode] = 0x1a; //0x0a;
        _vram[CGAData::registerPalette] = 0x0f; //0x30;
        _vram[CGAData::registerHorizontalTotal] = 57 /*114*/ - 1;
        _vram[CGAData::registerHorizontalDisplayed] = 40; //80;
        _vram[CGAData::registerHorizontalSyncPosition] = 45; //90;
        _vram[CGAData::registerHorizontalSyncWidth] = 10; // 16;
        _vram[CGAData::registerVerticalTotal] = 128 - 1;
        _vram[CGAData::registerVerticalTotalAdjust] = 6;
        _vram[CGAData::registerVerticalDisplayed] = 101;
        _vram[CGAData::registerVerticalSyncPosition] = 112;
        _vram[CGAData::registerInterlaceMode] = 2;
        _vram[CGAData::registerMaximumScanline] = 1;
        _vram[CGAData::registerCursorStart] = 6;
        _vram[CGAData::registerCursorEnd] = 7;
        _vram[CGAData::registerStartAddressHigh] = 0;
        _vram[CGAData::registerStartAddressLow] = 0;
        _vram[CGAData::registerCursorAddressHigh] = 0;
        _vram[CGAData::registerCursorAddressLow] = 0;
        _data.setTotals(238944, 910, 238875);
        _data.change(0, -_regs, _regs + 0x4000, &_cgaBytes[0]);

        _outputSize = _output->requiredSize();

        add(&_bitmap);
        add(&_animated);

        _animated.setDrawWindow(this);
        _animated.setRate(60);

        _frac = 0x600;
        _squares.allocate(0x8000);
        for (int i = 0; i < 0x8000; ++i) {
            int ii = i * 2;
            if (ii >= 0x8000)
                ii -= 0x10000;
            _squares[i] = ((ii*ii + (_frac / 2)) / _frac) & 0xfffe;
        }
        _mode = 1;
        static int modeIncrements[] = {1, 2, 4};

        int initialShift = 5;
        int initialGrid = 1 << initialShift;
        _maxX = 320;
        _maxY = 101;
        _itersX = ((_maxX + initialGrid - 1)/initialGrid)*initialGrid + 1;
        _itersY = ((_maxY + initialGrid - 1)/initialGrid)*initialGrid + 1;

        _iters.allocate(_itersX*_itersY);
        for (int i = 0; i < _itersX*_itersY; ++i)
            _iters[i] = 0xff;

        _totalIters = 0;
        _iteratedPixels = 0;
        for (int i = 0; i < 5; ++i)
            _blockCounts[i] = 0;

        // Coarse grid initially
        for (int yp = 0; yp < _itersY; yp += initialGrid)
            for (int xp = 0; xp < _itersX; xp += initialGrid)                                   
                mandelIters(xp, yp);
        // Progressively refine
        for (int yp = 0; yp < _itersY - 1; yp += initialGrid)
            for (int xp = 0; xp < _itersX - 1; xp += initialGrid)
                subdivide(xp, yp, initialShift);

        ////Check that image is the same as the one we get with no guessing
        //Array<Byte> vram2(0x4000);
        //for (int i = 0; i < 0x4000; ++i)
        //    vram2[i] = _vram[i];
        //for (int i = 0; i < _itersX*itersY; ++i)
        //    _iters[i] = 0xff;
        //for (int yp = 0; yp < maxY; ++yp) {
        //    for (int xp = 0; xp < maxX; ++xp)
        //        mandelIters(xp, yp);
        //}
        //for (int i = 0; i < 0x4000; ++i)
        //    _vram[i] ^= vram2[i];


        printf("%i %i\n", _totalIters, _iteratedPixels);
        for (int i = 0; i < 5; ++i)
            printf("%i\n", _blockCounts[i]);
    }
    ~MandelWindow() { join(); }
    void join() { _output->join(); }
    void create()
    {
        setText("CGA Mandelbrot");
        setInnerSize(_outputSize);
        _bitmap.setTopLeft(Vector(0, 0));
        _bitmap.setInnerSize(_outputSize);
        RootWindow::create();
        _animated.start();
    }
    virtual void draw()
    {
        _data.change(0, -_regs, _regs + 0x4000, &_cgaBytes[0]);
        _output->restart();
        _animated.restart();
    }
    BitmapWindow* outputWindow() { return &_bitmap; }
    CGAData* getData() { return &_data; }
    CGASequencer* getSequencer() { return &_sequencer; }
private:
    int iters(int x, int y) { return _iters[y*_itersX + x]; }
    // aub
    // vwp
    // cqd
    void subdivide(int xp, int yp, int s)
    {
        ++_blockCounts[s - 1];
        int z = 1 << s;
        int a = iters(xp, yp);
        int b = iters(xp + z, yp);
        int c = iters(xp, yp + z);
        int d = iters(xp + z, yp + z);
        if (a == b && a == c && a == d) {
            for (int y = 0; y < z; ++y)
                for (int x = 0; x < z; ++x) {
                    int i = iters(xp + x, yp + y);
                    plot(xp + x, yp + y, a);
                    _iters[(yp + y)*_itersX + xp + x] = i;
                    //if (xp + x < 320 && 100 + yp + y <= 200)
                    //    plot2(xp + x, 100 + yp + y, a);
                }

            //for (int x = 0; x < z; ++x)
            //    if (xp + x < 320 && 100 + yp <= 200)
            //        plot2(xp + x, 100 + yp, 1);
            //for (int y = 0; y < z; ++y)
            //    if (xp < 320 && 100 + yp + y <= 200)
            //        plot2(xp, 100 + yp + y, 1);

            return;
        }
        int h = z >> 1;

        mandelIters(xp + h, yp);
        if (s > 1 || yp < 100) {
            mandelIters(xp, yp + h);
            mandelIters(xp + h, yp + h);
            mandelIters(xp + z, yp + h);
        }

        if (s > 1) {
            subdivide(xp, yp, s - 1);
            subdivide(xp + h, yp, s - 1);
            if (yp + h <= 100) {
                mandelIters(xp + h, yp + z);
                subdivide(xp + h, yp + h, s - 1);
                subdivide(xp, yp + h, s - 1);
            }
        }
    }
    void plot(int xp, int yp, int i)
    {
        _iters[yp*_itersX + xp] = i;
        if (xp >= _maxX || yp >= _maxY)
            return;
        plot2(xp, yp+100, i);
        plot2(xp, 100-yp, i);
    }
    void plot2(int xp, int yp, int i)
    {
        static Byte colourTable[] = {
            0x00, 0xff, 0xff, 0xff, 0xff, 0xee, 0xee, 0xee, 0xee, 0xaa, 0xaa,
            0xaa, 0xbb, 0xbb, 0xbb, 0x99, 0x99, 0x99, 0x88, 0x88, 0x11, 0x11,
            0x33, 0x33, 0x22, 0x22, 0x66, 0x77, 0x55, 0x44, 0xcc, 0xdd, 0xff, 0x00, 0x00};
        static int modeMasks[] = {0x80, 0xc0, 0xf0};
        int p = ((yp & 1) << 13) + (yp >> 1)*80 + (xp >> 2);
        Byte m = modeMasks[_mode] >> ((xp & 3) << 1);
        _vram[p] = (_vram[p] & ~m) + (colourTable[i] & m);
    }
    int aFromXp(int xp)
    {
        return (((xp - 240) * _frac)*3/320) & -2;
    }
    int bFromYp(int yp)
    {
        return ((yp * _frac)*9/4/200) & -2;
    }
    void mandelIters(int xp, int yp)
    {
        if (iters(xp, yp) != 0xff)
            return;
        ++_iteratedPixels;
        int a = aFromXp(xp);
        int b = bFromYp(yp);

        int bb = _squares[(b >> 1) & 0x7fff];
        if (a < -_frac*3/4) {
            int a1a1 = _squares[((a + _frac) >> 1) & 0x7fff];
            if (a1a1 + bb <= _frac/16) {
                plot(xp, yp, 33);
                return;
            }
        }
        else {
            if (b <= static_cast<int>(_frac*sqrt(3)*3/8)) {
                int aa = _squares[(a >> 1) & 0x7fff];
                SInt16 c2 = aa + bb;
                SInt16 d = (8*c2 - 3*_frac) & 0xffff;
                SInt16 e = _squares[((c2 + d) >> 1) & 0x7fff] - _squares[((c2 - d) >> 1) & 0x7fff];
                if ((SInt16)(e + 4*a) <= 3*_frac/8) {
                    plot(xp, yp, 34);
                    return;
                }
            }
        }

        int x = a;
        int y = b;
        int i;
        for (i = 32; i > 0; --i) {
            int xx = _squares[(x >> 1) & 0x7fff];
            int yy = _squares[(y >> 1) & 0x7fff];
            int zz = xx + yy;
            zz &= 0xffff;
            if (zz > 0x1c00)
                break;
            y = (_squares[((x + y) >> 1) & 0x7fff] - zz) + b;
            x = a + xx - yy;
            ++_totalIters;
        }
        plot(xp, yp, i);
    }
    int _blockCounts[5];
    int _maxX;
    int _maxY;
    int _itersX;
    int _itersY;
    int _mode;
    Array<Byte> _iters;
    Array<Word> _squares;
    int _frac;
    CGAData _data;
    CGASequencer _sequencer;
    CGAOutput* _output;
    ConfigFile* _configFile;
    AnimatedWindow _animated;
    BitmapWindow _bitmap;
    Vector _outputSize;
    Array<Byte> _cgaBytes;
    Byte* _vram;
    int _regs;
    int _frame;
    int _totalIters;
    int _iteratedPixels;
};

class Program : public WindowProgram<MandelWindow>
{
public:
    void run()
    {
        ConfigFile configFile;
        configFile.addDefaultOption("cgaROM", String("5788005.u33"));
        configFile.addDefaultOption("fftWisdom", String("wisdom"));

        String configName = "default.config";
        if (_arguments.count() >= 2)
            configName = _arguments[1];
        File configPath(configName, true);
        configFile.load(configPath);
        FFTWWisdom<float> wisdom(File(configFile.get<String>("fftWisdom"),
            configPath.parent()));

        CGAOutput output(_window.getData(), _window.getSequencer(),
            _window.outputWindow());
        _window.setOutput(&output);

        _window.setConfig(&configFile, configPath);

        WindowProgram::run();
        _window.join();
    }
};
