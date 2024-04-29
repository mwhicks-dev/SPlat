#ifndef SPlat_EnvironmentInterface_h
#define SPlat_EnvironmentInterface_h

namespace SPlat {

    class EnvironmentInterface {

    public:

        virtual bool get_running() = 0;

        virtual void set_running(bool) = 0;

    };

}

#endif