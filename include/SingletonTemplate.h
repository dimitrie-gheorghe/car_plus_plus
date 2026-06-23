//
// Created by dimitrie on 23.06.2026.
//

#ifndef OOP_SINGLETONTEMPLATE_H
#define OOP_SINGLETONTEMPLATE_H

template <typename T>
class SingletonTemplate {
public:
    SingletonTemplate(const SingletonTemplate&) = delete;
    SingletonTemplate& operator=(const SingletonTemplate&) = delete;
    SingletonTemplate(SingletonTemplate&&) = delete;
    SingletonTemplate& operator=(SingletonTemplate&&) = delete;

    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    SingletonTemplate() = default;
    ~SingletonTemplate() = default;
};

#endif //OOP_SINGLETONTEMPLATE_H