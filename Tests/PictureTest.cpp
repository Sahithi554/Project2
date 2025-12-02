/**
 * @file PictureTest.cpp
 * @author Charles Owen
 * @author Anik Momtaz
 */

#include <pch.h>
#include "gtest/gtest.h"

#include <Picture.h>
#include <Actor.h>

TEST(PictureTest, Size)
{
    Picture picture;

    auto size = picture.GetSize();
    ASSERT_EQ(1500, size.GetWidth());
    ASSERT_EQ(800, size.GetHeight());

    picture.SetSize(wxSize(123, 456));
    size = picture.GetSize();
    ASSERT_EQ(123, size.GetWidth());
    ASSERT_EQ(456, size.GetHeight());
}

TEST(PictureTest, Iterator)
{
    Picture picture;

    auto iter1 = picture.begin();
    auto iter2 = picture.end();

    // Test to ensure an iterator on an empty picture works
    ASSERT_FALSE(iter1 != iter2);

    // Add some actors
    std::shared_ptr<Actor> actor1 = std::make_shared<Actor>(L"Bob");
    std::shared_ptr<Actor> actor2 = std::make_shared<Actor>(L"Ted");
    std::shared_ptr<Actor> actor3 = std::make_shared<Actor>(L"Carol");
    std::shared_ptr<Actor> actor4 = std::make_shared<Actor>(L"Alice");

    ASSERT_EQ(nullptr, actor1->GetPicture());

    picture.AddActor(actor1);
    picture.AddActor(actor2);
    picture.AddActor(actor3);
    picture.AddActor(actor4);

    // Test the associations
    ASSERT_TRUE(actor1->GetPicture() == &picture);
    ASSERT_TRUE(actor2->GetPicture() == &picture);
    ASSERT_TRUE(actor3->GetPicture() == &picture);
    ASSERT_TRUE(actor4->GetPicture() == &picture);

    iter1 = picture.begin();
    iter2 = picture.end();
    ASSERT_TRUE(iter1 != iter2);
    ASSERT_TRUE(*iter1 == actor1);
    ++iter1;
    ASSERT_TRUE(iter1 != iter2);
    ASSERT_TRUE(*iter1 == actor2);
    ++iter1;
    ASSERT_TRUE(iter1 != iter2);
    ASSERT_TRUE(*iter1 == actor3);
    ++iter1;
    ASSERT_TRUE(iter1 != iter2);
    ASSERT_TRUE(*iter1 == actor4);
    ++iter1;

    // Ensure we are at the end
    ASSERT_FALSE(iter1 != iter2);
}

TEST(PictureTest, GetTimeline)
{
    Picture picture;

    Timeline *timeline = picture.GetTimeline();
    ASSERT_NE(nullptr, timeline);
}
