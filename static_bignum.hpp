#include<cstdint>
#include<iostream>
#include<algorithm> //std::copy std::move std::fill
using namespace std;

#ifndef BIGNUM_T_UINT_TYPE
#define BIGNUM_T_UINT_TYPE size_t
#endif 

template<size_t ARG_NUM_BITS>
class bignum_t
{
public:
	static const size_t NUM_BITS=ARG_NUM_BITS;
	typedef BIGNUM_T_UINT_TYPE UINT_TYPE;
	
	static const size_t TYPE_SIZE=sizeof(UINT_TYPE)*8;
	static const size_t MASK_SIZE=NUM_BITS % TYPE_SIZE;

	static const size_t ARRAY_SIZE=(NUM_BITS / TYPE_SIZE)+(MASK_SIZE > 0 ? 1 : 0);
	
	static const UINT_TYPE TOP_MASK=(static_cast<UINT_TYPE>(1) << MASK_SIZE)-static_cast<UINT_TYPE>(1);
	
	UINT_TYPE data[ARRAY_SIZE];
	
	bignum_t()
	{}
	
	template<class IntInitializer>
	bignum_t(IntInitializer init)
	{
		std::fill(data,data+ARRAY_SIZE,static_cast<UINT_TYPE>(0));
		size_t stinit=init;
		for(unsigned int i=0;i<=(sizeof(IntInitializer)/sizeof(UINT_TYPE));i++)
		{
			data[i] = static_cast<UINT_TYPE>(stinit & (~static_cast<UINT_TYPE>(0)));
			stinit >>= TYPE_SIZE==(sizeof(size_t)*8) ? 0 : TYPE_SIZE;
		}
		if(MASK_SIZE)
		{
			data[MASK_SIZE-1] &= TOP_MASK;
		}
	}
	
	//special copy constructor for other static bignum types.
	//specifically ones that share a base architecture.  
	//Maybe the base architecture should be a #define in order to prevent mixing and matching.
	//Maybe only overload for ones where its allowed.
	template<size_t ANB2>
	bignum_t(const bignum_t<ANB2>& o)
	{
		static const size_t NAS = bignum_t<ANB2>::ARRAY_SIZE < ARRAY_SIZE ? bignum_t<ANB2>::ARRAY_SIZE : ARRAY_SIZE;
		std::copy(o.data,o.data+NAS,data);
		if(NAS < ARRAY_SIZE)
		{
			std::fill(data+NAS,data+ARRAY_SIZE,static_cast<UINT_TYPE>(0));
		}
	}
	
	template<size_t ANB2>
	bignum_t(bignum_t<ANB2>&& o)
	{
		static const size_t NAS = bignum_t<ANB2>::ARRAY_SIZE < ARRAY_SIZE ? bignum_t<ANB2>::ARRAY_SIZE : ARRAY_SIZE;
		std::move(o.data,o.data+NAS,data);
		if(NAS < ARRAY_SIZE)
		{
			std::fill(data+NAS,data+ARRAY_SIZE,static_cast<UINT_TYPE>(0));
		}
	}
	
	
	const bignum_t<NUM_BITS>& operator+() const
	{
		return *this;
	}
	
	/* if optimizing for code size then this implementation sucks and can be replaced with ~+1*/
	bignum_t<NUM_BITS> operator-() const
	{
		bignum_t<NUM_BITS> tmp;
		UINT_TYPE carry=1;
		for(unsigned int i=0;i<ARRAY_SIZE;i++)
		{
			UINT_TYPE r=~data[i]+carry;
			carry = !r ? 1 : 0;
			tmp.data[i]=r;
		}
		return tmp;
	}
	bignum_t<NUM_BITS>& operator+=(UINT_TYPE carry)
	{
		for(unsigned int i=0;i<ARRAY_SIZE;i++)
		{
			UINT_TYPE r=data[i];
			UINT_TYPE rn=r+carry;
			carry = rn < r ? 1 : 0;
			data[i]=rn;
			if(!carry) return *this;
		}
		return *this;
	}
	bignum_t<NUM_BITS> operator>>(size_t shiftamount)
	{
		
	}
	/*
	a++,a--,++a,--a,
	>>=,>>,<<,<<= (unsigned int args)
	&,&=,|,|=,^,^= (only for equal sized args)
	+,+=,-,-= (both types)
	+a,-a,~a
	*,*=,/,/= (UINT_TYPE args)
	%,%= (UINT_TYPE args)
	divmod
	popcount
	*/
};

int main(int argc,char** argv)
{
	bignum_t<512> a(10);
	bignum_t<256> t(10);
	a+=10;
	std::cout << a.data[0] << std::endl;
	return 0;
}

