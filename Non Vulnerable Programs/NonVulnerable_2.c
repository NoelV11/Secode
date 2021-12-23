Null Reference

#pragma once

#include "buy.h"
#include "constants.h"
#include "cross.h"
#include "order.h"
#include "sell.h"
#include "types.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>


namespace SCW
{

template< typename T >
void ASSERT_NO_ADD_OVERFLOW( T x, T y )
{
	assert( x <= (std::numeric_limits< T >::max() - y) );
}


template< typename DirtyPagesT, typename BookT >
void onDirtyPages( const DirtyPagesT & dirtyPages, BookT & book,
		Bookmarks & bookmarks )
{
	auto isCompleted = [&bookmarks]( const auto & pOrder )
	{
		if (pOrder->quantity)
		{
			return false;
		}

		assert( bookmarks.find( pOrder->id ) != std::end( bookmarks ) );
		bookmarks.erase( pOrder->id );

		return true;
	};

	auto prune = [&book, &isCompleted]( auto offset )
	{
		auto & page = book[offset];

		page.erase( std::remove_if(
					std::begin( page ), std::end( page ), isCompleted ),
				page.end() );
	};

	std::for_each( std::begin( dirtyPages ), std::end( dirtyPages ), prune );
}


template< typename OrderT, typename BookT >
std::set< typename BookT::size_type >
trade( std::ostream & ostream, OrderPtr< OrderT > & pLhs, BookT & book,
		Crosses< BookT > & crosses )
{
	auto dirtyPages = std::set< typename BookT::size_type >{};

	// Trade price crossings
	std::for_each( std::begin( crosses ), std::end( crosses ),
			[&]( const typename Crosses< BookT >::value_type & cross )
	{
		if (cross.seq == 0 || cross.priceOffset == 0 || cross.quantity == 0)
		{
			return;
		}

		auto & pRhs = book[cross.priceOffset][cross.seqOffset];

		assert( pLhs->quantity >= cross.quantity );
		assert( pRhs->quantity >= cross.quantity );

		pLhs->quantity -= cross.quantity;
		pRhs->quantity -= cross.quantity;

		if (!pRhs->quantity)
		{
			dirtyPages.insert( cross.priceOffset );
		}

		ostream << "TRADE ";
		ostream << pRhs->id << " " << pRhs->price << " " << cross.quantity;
		ostream << " ";
		ostream << pLhs->id << " " << pLhs->price << " " << cross.quantity;
		ostream << std::endl;
	});


	return dirtyPages;
}


// Process orders
template< typename OrderT, typename BookT, typename BookS >
void process( std::istream & istream, std::ostream & ostream,
		BookT & ourBook, BookS & otherBook, Bookmarks & bookmarks )
{
	auto pOrder = parse< OrderT >( istream );

	auto crosses = pOrder->run( otherBook );

	auto dirtyPages = trade( ostream, pOrder, otherBook, crosses );

	if (pOrder->quantity && pOrder->isGFD)
	{
		bookmarks[pOrder->id] = std::make_pair( pOrder->price, pOrder->seq );

		// Already using sparse arrays, invalid $0 at Book[0] for easy indexing
		auto & page = ourBook[pOrder->price];
		page.emplace_back( std::move( pOrder ) );
	}

	onDirtyPages( dirtyPages, otherBook, bookmarks );
}


// Print book
template< typename BookT >
void print( std::ostream & ostream, const BookT & book )
{
	// Header
	ostream << BookT::value_type::value_type::element_type::Code <<
		":" << std::endl;

	auto foldLeft = []( auto eax, const auto & pOrder )
	{
		ASSERT_NO_ADD_OVERFLOW< decltype( eax ) >( eax, pOrder->quantity );
		return eax + pOrder->quantity;
	};

	auto printPage = [&foldLeft, &ostream]( const auto & page )
	{
		const auto & begin = std::cbegin( page );
		const auto & end = std::cend( page );

		if (begin == end)
		{
			return;
		}

		// std::reduce in C++17 avoids folding left for parallelization
		auto quantity = std::accumulate( begin, end, 0u, foldLeft );
		if (quantity)
		{
			ostream << page.front()->price << " " << quantity << std::endl;
		}
	};

    std::for_each( std::rbegin( book ), std::rend( book ), printPage );
}


bool cancel( std::istream & istream, Buys & buys, Sells & sells,
		Bookmarks & bookmarks );

bool cancel( std::istream & istream, Buys & buys, Sells & sells,
		Bookmarks & bookmarks )
{
	auto id = std::string{};
	istream >> id;

	auto it = bookmarks.find( id );
	if (it == std::end( bookmarks ))
	{
		return false;
	}

	auto & bookmark = it->second;

	auto hasID = [&id]( const auto & pOrder )
	{
		return pOrder->id == id;
	};

	auto searchPage = [&bookmark, &bookmarks, &hasID, &id]( auto & book )
	{
		auto & page = book[bookmark.first];
		auto end = std::end( page );

		auto orderIt = std::find_if( std::begin( page ), end, hasID );
		if (orderIt == end)
		{
			return false;
		}

		page.erase( orderIt );
		bookmarks.erase( id );

		return true;
	};

	return searchPage( buys ) || searchPage( sells );
}


void matcherEngine( std::istream & istream, std::ostream & ostream );

// Parse lines for operations
void matcherEngine( std::istream & istream, std::ostream & ostream )
{
	auto token = std::string{};

	istream >> std::skipws;
	
	auto bookmarks = Bookmarks{};

	// [0, MaxPrice]
	auto buys = Buys{ MaxPrice + 1 };
	auto sells = Sells{ MaxPrice + 1 };

	while (istream >> token)
	{
		if (token == Buy::Code)
		{
			process< Buy >( istream, ostream, buys, sells, bookmarks );
		}
		else if (token == Sell::Code)
		{
			process< Sell >( istream, ostream, sells, buys, bookmarks );
		}
		else if (token == CANCEL)
		{
			cancel( istream, buys, sells, bookmarks );
		}
		else if (token == PRINT)
		{
			print( ostream, sells );
			print( ostream, buys );
		}
	}
}

} // namespace SCW


#ifndef CATCH_CONFIG_MAIN
int main()
{
	SCW::matcherEngine( std::cin, std::cout );
	return 0;
}
#endif

Solution

#pragma once

#include "buy.h"
#include "constants.h"
#include "cross.h"
#include "order.h"
#include "sell.h"
#include "types.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>


namespace SCW
{

template< typename T >
void ASSERT_NO_ADD_OVERFLOW( T x, T y )
{
	assert( x <= (std::numeric_limits< T >::max() - y) );
}


template< typename DirtyPagesT, typename BookT >
void onDirtyPages( const DirtyPagesT & dirtyPages, BookT & book,
		Bookmarks & bookmarks )
{
	auto isCompleted = [&bookmarks]( const auto & pOrder )
	{
		if (pOrder->quantity)
		{
			return false;
		}

		assert( bookmarks.find( pOrder->id ) != std::end( bookmarks ) );
		bookmarks.erase( pOrder->id );

		return true;
	};

	auto prune = [&book, &isCompleted]( auto offset )
	{
		auto & page = book[offset];

		page.erase( std::remove_if(
					std::begin( page ), std::end( page ), isCompleted ),
				page.end() );
	};

	std::for_each( std::begin( dirtyPages ), std::end( dirtyPages ), prune );
}


template< typename OrderT, typename BookT >
std::set< typename BookT::size_type >
trade( std::ostream & ostream, OrderPtr< OrderT > & pLhs, BookT & book,
		Crosses< BookT > & crosses )
{
	auto dirtyPages = std::set< typename BookT::size_type >{};

	// Trade price crossings
	std::for_each( std::begin( crosses ), std::end( crosses ),
			[&]( const typename Crosses< BookT >::value_type & cross )
	{
		if (cross.seq == 0 || cross.priceOffset == 0 || cross.quantity == 0)
		{
			return;
		}

		auto & pRhs = book[cross.priceOffset][cross.seqOffset];

		assert( pLhs->quantity >= cross.quantity );
		assert( pRhs->quantity >= cross.quantity );

		pLhs->quantity -= cross.quantity;
		pRhs->quantity -= cross.quantity;

		if (!pRhs->quantity)
		{
			dirtyPages.insert( cross.priceOffset );
		}

		ostream << "TRADE ";
		ostream << pRhs->id << " " << pRhs->price << " " << cross.quantity;
		ostream << " ";
		ostream << pLhs->id << " " << pLhs->price << " " << cross.quantity;
		ostream << std::endl;
	});


	return dirtyPages;
}


// Process orders
template< typename OrderT, typename BookT, typename BookS >
void process( std::istream & istream, std::ostream & ostream,
		BookT & ourBook, BookS & otherBook, Bookmarks & bookmarks )
{
	auto pOrder = parse< OrderT >( istream );

	if (!pOrder)
	{
		return;
	}

	auto crosses = pOrder->run( otherBook );

	auto dirtyPages = trade( ostream, pOrder, otherBook, crosses );

	if (pOrder->quantity && pOrder->isGFD)
	{
		bookmarks[pOrder->id] = std::make_pair( pOrder->price, pOrder->seq );

		// Already using sparse arrays, invalid $0 at Book[0] for easy indexing
		auto & page = ourBook[pOrder->price];
		page.emplace_back( std::move( pOrder ) );
	}

	onDirtyPages( dirtyPages, otherBook, bookmarks );
}


// Print book
template< typename BookT >
void print( std::ostream & ostream, const BookT & book )
{
	// Header
	ostream << BookT::value_type::value_type::element_type::Code <<
		":" << std::endl;

	auto foldLeft = []( auto eax, const auto & pOrder )
	{
		ASSERT_NO_ADD_OVERFLOW< decltype( eax ) >( eax, pOrder->quantity );
		return eax + pOrder->quantity;
	};

	auto printPage = [&foldLeft, &ostream]( const auto & page )
	{
		const auto & begin = std::cbegin( page );
		const auto & end = std::cend( page );

		if (begin == end)
		{
			return;
		}

		// std::reduce in C++17 avoids folding left for parallelization
		auto quantity = std::accumulate( begin, end, 0u, foldLeft );
		if (quantity)
		{
			ostream << page.front()->price << " " << quantity << std::endl;
		}
	};

    std::for_each( std::rbegin( book ), std::rend( book ), printPage );
}


bool cancel( std::istream & istream, Buys & buys, Sells & sells,
		Bookmarks & bookmarks );

bool cancel( std::istream & istream, Buys & buys, Sells & sells,
		Bookmarks & bookmarks )
{
	auto id = std::string{};
	istream >> id;

	auto it = bookmarks.find( id );
	if (it == std::end( bookmarks ))
	{
		return false;
	}

	auto & bookmark = it->second;

	auto hasID = [&id]( const auto & pOrder )
	{
		return pOrder->id == id;
	};

	auto searchPage = [&bookmark, &bookmarks, &hasID, &id]( auto & book )
	{
		auto & page = book[bookmark.first];
		auto end = std::end( page );

		auto orderIt = std::find_if( std::begin( page ), end, hasID );
		if (orderIt == end)
		{
			return false;
		}

		page.erase( orderIt );
		bookmarks.erase( id );

		return true;
	};

	return searchPage( buys ) || searchPage( sells );
}


void matcherEngine( std::istream & istream, std::ostream & ostream );

// Parse lines for operations
void matcherEngine( std::istream & istream, std::ostream & ostream )
{
	auto token = std::string{};

	istream >> std::skipws;
	
	auto bookmarks = Bookmarks{};

	// [0, MaxPrice]
	auto buys = Buys{ MaxPrice + 1 };
	auto sells = Sells{ MaxPrice + 1 };

	while (istream >> token)
	{
		if (token == Buy::Code)
		{
			process< Buy >( istream, ostream, buys, sells, bookmarks );
		}
		else if (token == Sell::Code)
		{
			process< Sell >( istream, ostream, sells, buys, bookmarks );
		}
		else if (token == CANCEL)
		{
			cancel( istream, buys, sells, bookmarks );
		}
		else if (token == PRINT)
		{
			print( ostream, sells );
			print( ostream, buys );
		}
	}
}

} // namespace SCW


#ifndef CATCH_CONFIG_MAIN
int main()
{
	SCW::matcherEngine( std::cin, std::cout );
	return 0;
}
#endif
