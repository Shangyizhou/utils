#ifndef REDIS_REPYL_H
#define REDIS_REPYL_H

#include <vector>
#include <string>

class redisReply;
class Reply
{
public:
    /**
     * @brief Define reply type
     */
    enum type_t {
        STRING = 1,
        ARRAY = 2,
        INTEGER = 3,
        NIL = 4,
        STATUS = 5,
        ERROR = 6
    };

    Reply(redisReply *reply);
    Reply();

    inline type_t type() const { return m_type; }

    inline const std::string& str() const { return m_str; }

    inline long long integer() const { return m_integer; }

    inline const std::vector<Reply>& elements() const { return m_elements; }

private:
    type_t m_type;
    std::string m_str;
    long long m_integer;
    std::vector<Reply> m_elements;
};




#endif // REDIS_REPYL_H