#ifndef SPARCESET_H
#define SPARCESET_H

#include <vector>
#include <type_traits>
#include <limits>


namespace ssecs
{
namespace tools
{


#define SSECS_SPARCESET_PAGE_SIZE 32768;

template<typename uType, typename = std::enable_if_t<std::is_unsigned_v<uType>>>
class SparceSet
{
   using size_type = std::size_t;
   static constexpr auto _undefined = std::numeric_limits<std::size_t>::max();
private:
   std::vector<uType> m_sparce;
   std::vector<uType> m_dense;

public:
   SparceSet() = default;
   SparceSet(const SparceSet& sparceSet) = delete;
   ~SparceSet() = default;

   void insert(uType value)
   {
       if(m_sparce.size() <= value)
       {
           m_sparce.resize(value+1);
       }
       m_sparce.back() = m_dense.size();
       m_dense.push_back(value);
   }
   bool has(uType value)
   {
       return m_sparce.size() < static_cast<size_t>(value) && m_sparce[static_cast<std::size_t>(value)] != _undefined;
   }
   uType& get(uType value)
   {
       return m_dense[m_sparce[static_cast<std::size_t>(value)]];
   }

   const uType& get(uType value) const
   {
       return m_dense[m_sparce[static_cast<std::size_t>(value)]];
   }

   std::vector<uType>& data() const
   {
       return m_dense;
   }

   std::size_t size() const
   {
       return m_dense.size();
   }

   void reserve(const size_t cap)
   {
       m_sparce.reserve(cap);
   }

   size_t capacity() const
   {
       return m_sparce.size();
   }

   bool empty() const
   {
       return m_dense.empty();
   }

   void erase(uType value)
   {
       m_dense.erase(m_sparce[static_cast<std::size_t>(value)]);
       m_sparce[static_cast<std::size_t>(value)] = _undefined;
   }


};
}
}



#endif // SPARCESET_H
