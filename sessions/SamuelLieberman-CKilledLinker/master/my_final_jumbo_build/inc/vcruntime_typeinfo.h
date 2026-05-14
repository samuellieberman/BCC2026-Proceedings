/* Winelib does not have headers for the Windows C++ standard library. See
 * this StackOverflow page where someone else encountered this fact.
 * https://stackoverflow.com/questions/47946016/problems-compiling-c-programs-with-wineg-winelib
 * 
 * The implementation was copied from the ABI_MICROSOFT NO_VSRUNTIME
 * declarations in libc++'s typeinfo header. See https://llvm.org/LICENSE.txt
 * for its license information.
 */

#ifndef SUPPLEMENTARY_VCRUNTIME_TYPEINFO_H
#define SUPPLEMENTARY_VCRUNTIME_TYPEINFO_H

#ifndef __cplusplus
#error This is a C++ header.
#endif
/*
namespace std // purposefully not using versioning namespace
{

class _LIBCPP_EXPORTED_FROM_ABI type_info {
  type_info& operator=(const type_info&);
  type_info(const type_info&);

  mutable struct {
    const char* __undecorated_name;
    const char __decorated_name[1];
  } __data;

  int __compare(const type_info& __rhs) const _NOEXCEPT;

public:
  virtual ~type_info();

  const char* name() const _NOEXCEPT;

  _LIBCPP_HIDE_FROM_ABI bool before(const type_info& __arg) const _NOEXCEPT { return __compare(__arg) < 0; }

  size_t hash_code() const _NOEXCEPT;

  _LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX23 bool operator==(const type_info& __arg) const _NOEXCEPT {
    // When evaluated in a constant expression, both type infos simply can't come
    // from different translation units, so it is sufficient to compare their addresses.
    if (__libcpp_is_constant_evaluated()) {
      return this == &__arg;
    }
    return __compare(__arg) == 0;
  }

#      if _LIBCPP_STD_VER <= 17
  _LIBCPP_HIDE_FROM_ABI bool operator!=(const type_info& __arg) const _NOEXCEPT { return !operator==(__arg); }
#      endif
};*/


_LIBCPP_BEGIN_UNVERSIONED_NAMESPACE_STD

/*class bad_cast : public exception {
public:
  bad_cast() _NOEXCEPT : exception("bad cast") {}

private:
  bad_cast(const char* const __message) _NOEXCEPT : exception(__message) {}
};*/

/*class bad_typeid : public exception {
public:
  bad_typeid() _NOEXCEPT : exception("bad typeid") {}

private:
  bad_typeid(const char* const __message) _NOEXCEPT : exception(__message) {}
};*/

_LIBCPP_END_UNVERSIONED_NAMESPACE_STD

//} // namespace std

#endif // SUPPLEMENTARY_VCRUNTIME_TYPEINFO_H