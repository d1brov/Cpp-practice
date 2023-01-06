#include "pch.h"

int GetProductsAmount(const std::vector<int>& prices, int amountOfMoney)
{
	std::vector<int> sortedPrices(prices); // added temporal sorted vector
	std::sort(sortedPrices.begin(), sortedPrices.end());

	int sum = 0;
	for (int i = 0; i < sortedPrices.size(); ++i)
	{
		sum += sortedPrices[i];
		if (sum > amountOfMoney)
		{
			return i;
		}
	}
	return prices.size();
}

TEST(GetProductsAmount, ValidPrices_MoneyForOneProduct_returnsOne) {
	std::vector<int> prices{ 1, 4, 3, 2 };

	auto result = GetProductsAmount(prices, 1);

	EXPECT_EQ(result, 1);
}

TEST(GetProductsAmount, ValidPrices_ValidMoneyAmount_returnsValidAmount) {
	std::vector<int> prices{1, 4, 3, 2};

	auto result = GetProductsAmount(prices, 6);

	EXPECT_EQ(result, 3);
}

TEST(GetProductsAmount, ValidPrices_MoneyForAllProducts_returnsAllAmount) {
	std::vector<int> prices{ 1, 2, 3, 4 };

	auto result = GetProductsAmount(prices, 10);

	EXPECT_EQ(result, 4);
}

TEST(GetProductsAmount, ValidPrices_ZeroMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{ 1, 2, 3, 4 };

	auto result = GetProductsAmount(prices, 0);

	EXPECT_EQ(result, 0);
}

TEST(GetProductsAmount, ValidPrices_NegativeMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{ 1, 2, 3, 4 };

	auto result = GetProductsAmount(prices, -10);

	EXPECT_EQ(result, 0);
}



TEST(GetProductsAmount, EmptyPrices_ValidMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{};

	auto result = GetProductsAmount(prices, 2);

	EXPECT_EQ(result, 0);
}

TEST(GetProductsAmount, EmptyPrices_ZeroMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{};

	auto result = GetProductsAmount(prices, 0);

	EXPECT_EQ(result, 0);
}

TEST(GetProductsAmount, EmptyPrices_NegativeMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{};

	auto result = GetProductsAmount(prices, -10);

	EXPECT_EQ(result, 0);
}



TEST(GetProductsAmount, ZeroPrices_ValidMoneyAmount_returnsAllAmount) {
	std::vector<int> prices{0, 0, 0, 0};

	auto result = GetProductsAmount(prices, 1);

	EXPECT_EQ(result, prices.size());
}

TEST(GetProductsAmount, ZeroPrices_ZeroMoneyAmount_returnsAllAmount) {
	std::vector<int> prices{ 0, 0, 0, 0 };

	auto result = GetProductsAmount(prices, 0);

	EXPECT_EQ(result, prices.size());
}

TEST(GetProductsAmount, ZeroPrices_NegativeMoneyAmount_returnsZeroAmount) {
	std::vector<int> prices{ 0, 0, 0, 0 };

	auto result = GetProductsAmount(prices, -10);

	EXPECT_EQ(result, 0);
}



TEST(GetProductsAmount, NegativePrices_ValidMoneyAmount_returnsAllAmount) {
	std::vector<int> prices{ -1, -2, -3, -4 };

	auto result = GetProductsAmount(prices, 1);

	EXPECT_EQ(result, prices.size());
}

TEST(GetProductsAmount, NegativePrices_ZeroMoneyAmount_returnsAllAmount) {
	std::vector<int> prices{ -1, -2, -3, -4 };

	auto result = GetProductsAmount(prices, 0);

	EXPECT_EQ(result, prices.size());
}

TEST(GetProductsAmount, NegativePrices_NegativeMoneyAmount_returnsAllAmount) {
	std::vector<int> prices{ -1, -2, -3, -4 };

	auto result = GetProductsAmount(prices, -1);

	EXPECT_EQ(result, prices.size());
}