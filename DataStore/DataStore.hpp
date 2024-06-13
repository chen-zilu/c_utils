#ifndef _DATA_STORE_H_
#define _DATA_STORE_H_

// 未支持多线程
template<typename T>
class DataStore{
    public:
        T *buf;     //环形缓冲区
        unsigned int len;    //缓冲区长度
        int buf_cnt;   //循环计数

        double timeout_max;      //超时计数最大值，由超时时间和间隔计算出来
        unsigned int timeout_cnt;   //超时计数
        bool stabled;         //数据稳定标志

        bool data_vld;   // 数据有效

    public:
        DataStore();
        ~DataStore();
        int init(unsigned int _len, unsigned int _timeout_max);
        void set(T data);       //保存数据
        T get(int idx);  //获取数据,-1为最旧值，0为最新值
        void check_timeout();   //检查超时
        bool is_newdata();
        bool is_stable();        //判断数据稳定
};


/**
 * @brief 默认构造函数
*/
template<typename T>
DataStore<T>::DataStore()
{
    len = 0;
    buf = nullptr;
    buf_cnt = 0;

    timeout_max = 0;
    timeout_cnt = 0;
    stabled = false;

    data_vld = false;
}

/**
 * @brief 初始化
 * @param _len 缓冲区长度
 * @param _timeout_max 超时阈值计数
 * @return 0：成功 -1：失败
 * 
*/
template<typename T>
int DataStore<T>::init(unsigned int _len, unsigned int _timeout_max)
{
    if(_len == 0)
        return -1;

    len = _len;
    if(buf == nullptr)
        buf = new T[len];
    else
    {
        delete[] buf;
        buf = new T[len];
    }
    buf_cnt = 0;
    data_vld = false;

    timeout_max = _timeout_max;
    timeout_cnt = 0;
    stabled = false;
    return 0;
}

/**
 * @brief  delete buf
*/
template<typename T>
DataStore<T>::~DataStore()
{
    delete[] buf;
}

/**
 * @brief 保存数据，清空超时计数，成功保存len个数据视为数据稳定
*/
template<typename T>
void DataStore<T>::set(T data)
{
    buf[buf_cnt] = data;
    data_vld = true;

    buf_cnt++;
    if( buf_cnt >= len )
    {
        buf_cnt = 0;
        // 数据已稳定
        if( stabled == false )
            stabled = true;
    }

    // 清空超时计数
    timeout_cnt = 0;

}

/**
 * @brief 检查是否超时，超时会重置
*/
template<typename T>
void DataStore<T>::check_timeout()
{
    timeout_cnt++;
    // 数据超时
    if( timeout_cnt >= timeout_max )
    {
        buf_cnt = 0;
        stabled = false;
    }
}


/**
 * @brief 检查新数据
*/
template<typename T>
bool DataStore<T>::is_newdata()
{
    if(data_vld == false)
        return false;
    else
    {
        data_vld = false;
        return true;
    }
}

/**
 * @brief 判断数据稳定
*/
template<typename T>
bool DataStore<T>::is_stable()
{
    return stabled;
}

/**
 * @brief 获取buf数据,-1为最旧值，0为最新值
 * @param idx -1为最旧值，0为最新值，按len取余，注意不判断stable！！
 * @return 返回数据
*/
template<typename T>
T DataStore<T>::get(int idx)
{
    int tmp = buf_cnt-1-idx;
    if( tmp >= 0)
        return buf[ tmp%len ];
    else
        return buf[ len - (-tmp)%len];
}

#endif