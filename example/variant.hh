/**************************************************************************/
/*
   Autogenerated by the Variant Generator.

   Call was:

     '../build/mkvariant' 'variant.inf'
                                                                          */
/**************************************************************************/
#ifndef VARIANT_HH
#define VARIANT_HH

#include <stdexcept>
#include <stdint.h>
#include <string>

class Variant {
  private:
    uint8_t tag_ {0};
    union {
      std::string std_string_;
      uint32_t uint32_t_;
      int32_t int32_t_;
    };
    void destruct();
  public:
    Variant();
    Variant(Variant &&o);
    Variant(const Variant &o) =delete;
    Variant(std::string &&o);
    Variant(uint32_t o);
    Variant(int32_t o);
    ~Variant();

    Variant &operator=(Variant &&o);
    Variant &operator=(const Variant &o) =delete;
    Variant &operator=(std::string &&o);
    Variant &operator=(uint32_t o);
    Variant &operator=(int32_t o);
    template <typename T> typename T::result_type apply(T &t)
    {
      switch (tag_) {
        case 1 : return t(std_string_);
        case 2 : return t(uint32_t_);
        case 3 : return t(int32_t_);
      }
      throw std::domain_error("variant not initialized");
    }

    template <typename T> typename T::result_type apply(const T &t)
    {
      switch (tag_) {
        case 1 : return t(std_string_);
        case 2 : return t(uint32_t_);
        case 3 : return t(int32_t_);
      }
      throw std::domain_error("variant not initialized");
    }

    template <typename T> typename T::result_type accept(T &t) const
    {
      switch (tag_) {
        case 1 : return t(std_string_);
        case 2 : return t(uint32_t_);
        case 3 : return t(int32_t_);
      }
      throw std::domain_error("variant not initialized");
    }

    template <typename T> typename T::result_type accept(const T &t) const
    {
      switch (tag_) {
        case 1 : return t(std_string_);
        case 2 : return t(uint32_t_);
        case 3 : return t(int32_t_);
      }
      throw std::domain_error("variant not initialized");
    }

};



#endif
