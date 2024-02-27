#ifndef HOSHIMNET_BASE_SINGLETON_H_
#define HOSHIMNET_BASE_SINGLETON_H_

namespace hoshiMNet
{
namespace base
{

template<typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }
protected:
    Singleton() {}
    virtual ~Singleton() {}
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&&) = delete;
};

// test
// class Test : public Singleton<Test>
// {
// friend class Singleton<Test>;

// private:
//     Test() {}
//     ~Test() {}
// };

} // namespace base
} // namespace hoshiMNet

#endif // HOSHIMNET_BASE_SINGLETON_H_