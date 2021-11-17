#include "library.h"


namespace Laba3{


    Binary_signal::Binary_signal(const unsigned char num, const double duration) : max_duration(duration),
                                                                                SZ(QUOTA), SZ_now(QUOTA){
        status = new Signal_status[QUOTA];
        status[0].value = num;
        status[0].seconds = max_duration;

    }

    Binary_signal::Binary_signal(const std::string &string,const double time,const double duration) :
                                                                    status(new Signal_status[QUOTA])
    {
        int sum = string.length();

        if(time > duration) {
            delete [] status;
            throw std::invalid_argument("Error with time");
        }

        max_duration = duration;
        SZ = QUOTA;

        if(sum > QUOTA) {

            int n = sum / QUOTA;
            if(sum % QUOTA != 0)
                n ++;
            status = new Signal_status [QUOTA * n];
            SZ = QUOTA * n;
        }
        double seconds = time / (double) sum;
        for(int i = 0; i < sum; i++){
            if(string[i] != '0' && string[i] != '1')
                throw std::invalid_argument("Incorrect value!");
            status[i].value = string[i];
            status[i].seconds = seconds;
        }
        SZ_now = sum;
    }

    Binary_signal::Binary_signal(const Binary_signal &signal) : SZ(signal.SZ), SZ_now(signal.SZ_now),
                                                                max_duration(signal.max_duration) {

        status = new Signal_status[SZ];
        for(int i = 0; i < SZ_now; i++){
            status[i].value = signal.status[i].value;
            status[i].seconds = signal.status[i].seconds;
        }

    }


    Binary_signal::Binary_signal(Binary_signal &&signal) : SZ(signal.SZ), SZ_now(signal.SZ_now),
                                                max_duration(signal.max_duration), status(signal.status)
    {
        signal.status = nullptr;
    }

    Binary_signal &Binary_signal::operator = (const Binary_signal &signal) {

        if(this != &signal){
            SZ = signal.SZ;
            SZ_now = signal.SZ_now;
            max_duration = signal.max_duration;
            delete [] status;
            status = new Signal_status[SZ];
            for(int i = 0; i < SZ_now; i++){
                status[i].value = signal.status[i].value;
                status[i].seconds = signal.status[i].seconds;
            }
        }
        return *this;
    }

    Binary_signal &Binary_signal::operator = (Binary_signal &&signal) {
        if(&signal == this)
            return *this;

        std::swap(SZ, signal.SZ);
        std::swap(SZ_now, signal.SZ_now);
        std::swap(max_duration, signal.max_duration);
        std::swap(status, signal.status);

        return *this;
    }

    std::ostream & operator << (std::ostream & s,const Binary_signal &signal){

        for (int i = 0; i < signal.get_SZ_now(); i++) {

            s << signal.status[i].value;

        }
        return s;

    }

    std::istream &operator>>(std::istream &s, Binary_signal &signal) {
        std::string some_string;
        double duration;
        double time;

        s >> duration;

        s >> some_string;

        s >> time;


        signal = Binary_signal(some_string,time,duration);

        return s;

    }

    double Binary_signal::time_check() const{
        double count = 0;
        for(int i = 0;i < SZ_now;i++){
            if(status[i].seconds != 0)
                count += status[i].seconds;
        }
        return count;

    }


    void Binary_signal::signal_reorganization() {

        double signal_time = time_check();
        double ex_time = 0;
        for(int i = 0; i < SZ_now ; i++){
            if(ex_time < signal_time)
                if(status[i].seconds == 0){
                    for(int k = i; k < SZ_now + 1; k++){
                        if((k + 1) > SZ_now)
                            break;
                        Signal_status variable = status[k+1];
                        status[k] = status[k+1];
                        status[k+1] = variable;
                    }
                }
            ex_time += status[i].seconds;
        }

    }


    void Binary_signal::insert_to_time(double duration,const unsigned char symbol,const double time) {

        if(duration + time_check() > max_duration)
            throw std::invalid_argument("Time error, not enough free time!");

        if(symbol != '0' && symbol != '1')
            throw std::invalid_argument("Incorrect value!");

        if(SZ_now == SZ){

            Signal_status *new_signal = new Signal_status[SZ + QUOTA];
            for(int i = 0; i < SZ_now; i++){
                new_signal[i].value = status[i].value;
                new_signal[i].seconds = status[i].seconds;
            }
            delete [] status;
            status = new_signal;
            SZ += QUOTA;
        }


        int ind = find_signal(time);
        int sz = SZ;

        for(int i = sz - 1; i > ind; i--){

            Signal_status variable = status[i];
            status[i] = status[i-1];
            status[i-1] = variable;
        }

        status[ind].value = symbol;
        status[ind].seconds = duration;
        SZ_now ++;

    }

    int Binary_signal::find_signal(const double time) const {
        double count = 0;
        for(int i = 0; i < SZ_now; i++){
            count += status[i].seconds;
            if(time < count)
                return i;
        }
        return -1;
    }

    void Binary_signal::delete_to_time(const double time,const double how_much) {

        int ind = find_signal(time);
        if(ind == -1)
            throw std::invalid_argument("Signal didn't find!");

        if(how_much == -1){
            status[ind].value = '0';
            status[ind].seconds = 0;
            SZ_now --;
            signal_reorganization();
        }else{
            if(how_much > status[ind].seconds)
                throw std::invalid_argument("Error with time!");
            status[ind].seconds -= how_much;
        }


    }



    Binary_signal Binary_signal::operator ~ () {

        Binary_signal signal = *this;
        for (int i = 0; i < signal.get_SZ_now(); i++){
            if(signal.status[i].seconds != 0){
                if(signal.status[i].value == '0')
                    signal.status[i].value = '1';
                else
                    signal.status[i].value = '0';
            }
        }

        return signal;
    }

    Binary_signal &Binary_signal::operator += (const Binary_signal &signal_two) {

        if(signal_two.time_check() > max_duration - (double) time_check())
            throw std::invalid_argument("We cannot add this signal, time error!");

        if(signal_two.SZ_now > SZ - SZ_now){

            int free_places = SZ - SZ_now;
            int need_places = signal_two.SZ_now - free_places;
            int k = need_places / QUOTA;
            if(need_places % QUOTA != 0)
                k ++;
            Signal_status *new_signal = new Signal_status [SZ + QUOTA * k];

            for(int i = 0; i < SZ_now; i++){
                new_signal[i].value = status[i].value;
                new_signal[i].seconds = status[i].seconds;
            }
            delete [] status;
            status = new_signal;
            SZ = SZ + QUOTA * k;
        }

        for(int i = 0; i < signal_two.SZ_now; i++){
            if(signal_two.getSeconds(i) != 0) {
                status[SZ_now].value = signal_two.getValue(i);
                status[SZ_now].seconds = signal_two.getSeconds(i);
                SZ_now ++;
            }
        }
        return *this;
    }

    Binary_signal Binary_signal::operator +(const Binary_signal &signal) const {
        Binary_signal sum = *this;
        return sum += signal;
    }

    Binary_signal &Binary_signal::operator *=(const int n) {

        int sum = SZ_now * n;

        if(time_check() * n > max_duration)
            throw std::invalid_argument("Time limit!");

        if(sum > SZ){

            int k = sum / QUOTA;
            if(sum % QUOTA != 0)
                k ++;
            Signal_status *new_signal = new Signal_status [QUOTA * k];

            for(int i = 0; i < SZ_now; i++){
                new_signal[i].value = status[i].value;
                new_signal[i].seconds = status[i].seconds;
            }
            delete [] status;
            status = new_signal;
            SZ = QUOTA * k;

        }


        int old_size = SZ_now;

        for(int i = 0; i < n-1; i ++){
            for(int j = 0; j < old_size; j++){
                status[SZ_now] = status[j];
                SZ_now ++;
            }
        }
        return *this;
    }



    Binary_signal &Binary_signal::operator += (const Signal_status &st) {

        if(st.seconds + time_check() > max_duration)
            throw std::invalid_argument("Time error, not enough free time!");

        if(st.value != '0' && st.value != '1')
            throw std::invalid_argument("Incorrect value!");

        if(SZ_now == SZ){

            Signal_status *new_signal = new Signal_status[SZ + QUOTA];

            for(int i = 0; i < SZ_now; i++){
                new_signal[i].value = status[i].value;
                new_signal[i].seconds = status[i].seconds;
            }
            delete [] status;
            status = new_signal;
            SZ += QUOTA;
        }

        int place = SZ_now;
        status[place].value = st.value;
        status[place].seconds = st.seconds;
        SZ_now ++;


        return *this;
    }

    Binary_signal Binary_signal::operator+(const Signal_status &st) const {
        Binary_signal signal1 = *this;
        return signal1 += st;
    }

    Binary_signal &Binary_signal::operator--() {

        for(int i = 0;i < SZ_now ;i++){
            status[i].value = '0';
            status[i].seconds = 0;
        }
        SZ_now = 0;
        return *this;
    }

    Binary_signal Binary_signal::operator--(int) {
        Binary_signal signal = *this;
        --(*this);
        return signal;
    }


}