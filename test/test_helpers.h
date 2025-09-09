// test_helpers.h - Updated to fix template parameter type issues

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace psimpl {
namespace test {

// Helper template to convert between numeric types
template<typename TargetType, typename SourceType>
std::vector<TargetType> ConvertVector(const std::vector<SourceType>& source) {
    std::vector<TargetType> result;
    result.reserve(source.size());
    
    // Use transform with explicit cast for numeric conversions
    if constexpr (std::is_arithmetic_v<SourceType> && std::is_arithmetic_v<TargetType>) {
        std::transform(source.begin(), source.end(), 
                      std::back_inserter(result),
                      [](SourceType val) { return static_cast<TargetType>(val); });
    } else {
        // For non-arithmetic types, use direct copy if compatible
        std::copy(source.begin(), source.end(), std::back_inserter(result));
    }
    
    return result;
}

// Helper to test integer conversions with a given simplification function
// Now template on the vector's value type to accept both float and double
template<unsigned DIM, typename ValueType, typename SimplifyFunc>
void TestIntegerConversion(
    const std::vector<ValueType>& polyline,
    const std::vector<ValueType>& expected,
    const std::vector<ValueType>& rexpected,
    SimplifyFunc simplifyFunc,
    double tolerance,
    const std::string& testName = "")
{
    // Test with int
    {
        auto intPolyline = ConvertVector<int>(polyline);
        auto intExpected = ConvertVector<int>(expected);
        std::vector<int> intResult;
        
        simplifyFunc(intPolyline.begin(), intPolyline.end(),
                    tolerance, std::back_inserter(intResult));
        
        VERIFY_TRUE(intResult == intExpected);
    }
    
    // Test with unsigned int
    {
        auto uintPolyline = ConvertVector<unsigned>(polyline);
        auto uintExpected = ConvertVector<unsigned>(expected);
        std::vector<unsigned> uintResult;
        
        simplifyFunc(uintPolyline.begin(), uintPolyline.end(),
                    tolerance, std::back_inserter(uintResult));
        
        VERIFY_TRUE(uintResult == uintExpected);
    }
    
    // Test with unsigned int (reverse)
    {
        auto uintPolyline = ConvertVector<unsigned>(polyline);
        auto ruintExpected = ConvertVector<unsigned>(rexpected);
        std::vector<unsigned> ruintResult;
        
        simplifyFunc(uintPolyline.rbegin(), uintPolyline.rend(),
                    tolerance, std::back_inserter(ruintResult));
        
        VERIFY_TRUE(ruintResult == ruintExpected);
    }
}

// Overload for functions that take additional parameters (like repeat count)
// Now template on the vector's value type to accept both float and double
template<unsigned DIM, typename ValueType, typename SimplifyFunc>
void TestIntegerConversionWithRepeat(
    const std::vector<ValueType>& polyline,
    const std::vector<ValueType>& expected,
    const std::vector<ValueType>& rexpected,
    SimplifyFunc simplifyFunc,
    double tolerance,
    int repeat,
    const std::string& testName = "")
{
    // Test with int
    {
        auto intPolyline = ConvertVector<int>(polyline);
        auto intExpected = ConvertVector<int>(expected);
        std::vector<int> intResult;
        
        simplifyFunc(intPolyline.begin(), intPolyline.end(),
                    tolerance, repeat, std::back_inserter(intResult));
        
        VERIFY_TRUE(intResult == intExpected);
    }
    
    // Test with unsigned int
    {
        auto uintPolyline = ConvertVector<unsigned>(polyline);
        auto uintExpected = ConvertVector<unsigned>(expected);
        std::vector<unsigned> uintResult;
        
        simplifyFunc(uintPolyline.begin(), uintPolyline.end(),
                    tolerance, repeat, std::back_inserter(uintResult));
        
        VERIFY_TRUE(uintResult == uintExpected);
    }
    
    // Test with unsigned int (reverse)
    {
        auto uintPolyline = ConvertVector<unsigned>(polyline);
        auto ruintExpected = ConvertVector<unsigned>(rexpected);
        std::vector<unsigned> ruintResult;
        
        simplifyFunc(uintPolyline.rbegin(), uintPolyline.rend(),
                    tolerance, repeat, std::back_inserter(ruintResult));
        
        VERIFY_TRUE(ruintResult == ruintExpected);
    }
}

// Generic test runner for different container types
template<unsigned DIM, typename Container, typename SimplifyFunc>
void TestWithContainer(
    const Container& polyline,
    SimplifyFunc simplifyFunc,
    typename Container::value_type tolerance,
    const std::vector<int>& expectedKeys,
    const std::string& testName = "")
{
    Container result;
    
    simplifyFunc(polyline.begin(), polyline.end(), tolerance,
                std::back_inserter(result));
    
    ASSERT_TRUE(result.size() == expectedKeys.size() * DIM);
    VERIFY_TRUE(ComparePoints<DIM>(polyline.begin(), result.begin(), expectedKeys));
}

}} // namespace psimpl::test

#endif // TEST_HELPERS_H