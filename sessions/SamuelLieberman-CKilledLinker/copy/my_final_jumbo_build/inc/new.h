/* Winelib does not have headers for the Windows C++ standard library. See
 * this StackOverflow page where someone else encountered this fact.
 * https://stackoverflow.com/questions/47946016/problems-compiling-c-programs-with-wineg-winelib
 * 
 * The declarations were all written by me, with the details copied from
 * the Microsoft <new> header documentation here:
 * https://learn.microsoft.com/en-us/cpp/standard-library/new
 */

//void* operator new(std::size_t count, void* ptr) noexcept;