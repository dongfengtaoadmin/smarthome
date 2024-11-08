#include "fingerprint-device.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
    FingerprintDevice fingerDev("/dev/ttyUSB0");

    bool ok = fingerDev.handshake();
    cout << "handshake ok:" << ok << endl;

    cout << "Input record fingerprint pos : ";
    uint16_t pos;
    cin >> pos;
    sleep(5);

    // ok = fingerDev.recordFingerprint(pos);
    // cout << "record fingerprint ok:" << ok << endl;

    // sleep(1);

    // pos = 0;
    // ok = fingerDev.verfiyFingerprint(pos);
    // cout << "verfiy fingerprint ok : " << ok << ",pos : " << pos << endl;

    // sleep(1);

    // ok = fingerDev.deleteFingerprint(pos);
    // cout << "delete fingerprint ok : " << ok << endl;

    // sleep(1);
    // ok = fingerDev.verfiyFingerprint(pos);
    // cout << "verfiy fingerprint ok : " << ok << ",pos : " << pos << endl;

    // sleep(1);
    // ok = fingerDev.clearFingerprintLib();
    // cout << "clear fingerprint lib ok : " << ok << endl;

    return 0;
}
