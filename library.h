#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H
#include <iostream>
#include <string>

namespace Laba3 {

    struct Signal_status {

        unsigned char value;
        double seconds;

    };

    class Binary_signal {

    private:

        static const int QUOTA = 10;

        int SZ;

        int SZ_now = 0;

        double max_duration = 100;

        Signal_status *status;

        void signal_reorganization();



    public:

        Binary_signal() : status(new Signal_status[QUOTA]), SZ(QUOTA){};
        explicit Binary_signal(double m_duration) : max_duration(m_duration),SZ(QUOTA),status(new Signal_status[QUOTA]){};
        explicit Binary_signal(unsigned char num,double duration);
        explicit Binary_signal(const std::string &string,double time,double duration);

        Binary_signal(const Binary_signal&);
        Binary_signal(Binary_signal &&) ;

        Binary_signal & operator = (const Binary_signal &);
        Binary_signal & operator = (Binary_signal &&);

        ~Binary_signal() { delete [] status; };

        void setMax_duration(const double time){max_duration = time;};
        void setValue(const unsigned char m_value,const int ind) {status[ind].value = m_value;}
        void setSeconds(const double m_seconds,const int ind) {status[ind].seconds = m_seconds;}

        double getDuration() const {return max_duration;}
        int get_SZ() const {return SZ;}
        int get_SZ_now() const {return SZ_now;}
        unsigned char getValue (const int ind) const {return status[ind].value;}
        double getSeconds(const int ind) const {return status[ind].seconds;}

        int find_signal(double time) const;
        double time_check() const;


        friend std::ostream & operator <<(std::ostream & ,const Binary_signal &);
        friend std::istream & operator >>(std::istream &, Binary_signal &);


        void insert_to_time(double duration,unsigned char symbol,double time);
        void delete_to_time(double time, double how_much);


        Binary_signal & operator +=(const Signal_status &);
        Binary_signal  operator +(const Signal_status &) const;
        Binary_signal & operator *= (int n);
        Binary_signal & operator += (const Binary_signal &);
        Binary_signal  operator +(const Binary_signal &) const;
        Binary_signal  operator ~();
        Binary_signal & operator -- ();
        Binary_signal  operator -- (int);

    };






}
#endif
