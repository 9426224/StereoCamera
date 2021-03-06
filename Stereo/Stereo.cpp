#include <iostream>

#include "Device.h"
#include "Image.h"

using namespace std;
using namespace cv;

int main()
{
    Device device;
    if (!device.Init())
    {
        cout << "Device Init Failed!" << endl;
        return -1;
    }

    Image *image = new Image(device.pHandleEtronDI, device.pDevSelInfo);

    image->OpenNet();

    thread t1 = image->GetImageThread();
    thread t2 = image->ProcessThread();
    thread t3 = image->SerialPortThread();

    t1.join();
    t2.join();
    t3.join();
}
