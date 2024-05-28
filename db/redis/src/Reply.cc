#include "Reply.h"
#include "hiredis/hiredis.h"
#include <iostream>

Reply::Reply(redisReply* reply) 
    : m_type(type_t::ERROR),
      m_integer(0) {
    m_type = static_cast<type_t>(reply->type);
    
    switch (m_type) {
        case type_t::ERROR:
        case type_t::STRING:
        case type_t::STATUS:
            m_str = std::string(reply->str, reply->len);
            break;
        case type_t::INTEGER:
            m_integer = reply->integer;
            break;
        case type_t::ARRAY:
            for (size_t i = 0; i < reply->elements; i++) {
                m_elements.push_back(Reply(reply->element[i]));
            }
            break;
        default:
            break;
    }
}

Reply::Reply()
    : m_type(type_t::ERROR),
      m_integer(0) {
    m_elements.reserve(0);
}
