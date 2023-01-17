/**
 *  ElephantDSP.com Room Reverb
 *
 *  Copyright (C) 2023 Christian Voigt
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DonationSelector.h"

DonationSelector::DonationSelector()
{
    setText("Donate via in-app purchase");
    getRootMenu()->addItem("In-app purchase is unavailable!", false, false, {});
    setJustificationType(juce::Justification::centred);

    if (!juce::InAppPurchases::getInstance()->isInAppPurchasesSupported())
    {
        jassertfalse; // In-app purchases are only supported on iOS
        return;
    }

    donationProducts = juce::Array<DonationProduct>({DonationProduct{"donation05", "Retrieving price..."},
                                                     DonationProduct{"donation10", "Retrieving price..."},
                                                     DonationProduct{"donation20", "Retrieving price..."},
                                                     DonationProduct{"donation50", "Retrieving price..."}});

    juce::InAppPurchases::getInstance()->addListener(this);

    juce::StringArray identifiers;
    for (const auto& donationProduct : donationProducts)
        identifiers.add(donationProduct.identifier);
    juce::InAppPurchases::getInstance()->getProductsInformation(identifiers);
}

DonationSelector::~DonationSelector()
{
    juce::InAppPurchases::getInstance()->removeListener(this);
}

void DonationSelector::productsInfoReturned(const juce::Array<juce::InAppPurchases::Product>& products)
{
    if (products.isEmpty())
        return;

    // remove "unavailable" entry
    getRootMenu()->clear();

    for (auto& donationProduct : donationProducts)
    {
        for (int i = 0; i < products.size(); ++i)
        {
            if (donationProduct.identifier == products[i].identifier)
            {
                donationProduct.purchasePrice = products[i].price;
                getRootMenu()->addItem(donationProduct.purchasePrice, [=]
                                       { purchase(donationProduct.identifier); });
                break;
            }
        }
    }
}

void DonationSelector::purchase(const juce::String& identifier)
{
    juce::InAppPurchases::getInstance()->purchaseProduct(identifier);
    setText("Donate via in-app purchase");
}
