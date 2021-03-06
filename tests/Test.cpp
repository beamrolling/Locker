#include "../include/Locker.h"
#include "../include/PrimaryLockerRobot.h"
#include "../include/SmartLockerRobot.h"
#include <gtest/gtest.h>

using namespace std;

TEST(locker, should_return_ticket_when_store_bag_given_locker_is_not_full) {
  //given
  Locker locker(20);
  Bag bag(666);

  //when
  SaveBagResult result = locker.SaveBag(bag);

  //then
  EXPECT_EQ(0, result.err);
  EXPECT_NE(0, result.ticket.id);
  EXPECT_EQ(19, locker.remain);
}

TEST(locker, should_show_error_message_when_store_bag_given_locker_is_full){
  //given
  Locker locker(1);
  Bag bag(666);
  SaveBagResult temp_result = locker.SaveBag(bag);

  //when
  SaveBagResult result = locker.SaveBag(bag);

  //then
  EXPECT_EQ(1, result.err);
  EXPECT_EQ(0, result.ticket.id);
  EXPECT_EQ(0, locker.remain);
}

TEST(locker, should_return_bag_when_get_bag_given_valid_ticket){
  //given
  Locker locker(20);
  Bag bag(666);
  SaveBagResult save_bag_result = locker.SaveBag(bag);

  //when
  GetBagResult result = locker.GetBag(save_bag_result.ticket);

  //then
  EXPECT_EQ(0, result.err);
  EXPECT_EQ(666, result.bag.id);
  EXPECT_EQ(20, locker.remain);
}

TEST(locker, should_show_invalid_ticket_message_when_get_bag_given_duplicated_ticket){
  //given
  Locker locker(20);
  Bag bag(666);
  SaveBagResult save_bag_result = locker.SaveBag(bag);
  (void)locker.GetBag(save_bag_result.ticket);

  //when
  GetBagResult result = locker.GetBag(save_bag_result.ticket);

  //then
  EXPECT_EQ(1, result.err);
  EXPECT_EQ(0, result.bag.id);
  EXPECT_EQ(20, locker.remain);
}

TEST(locker, should_show_invalid_ticket_message_when_get_bag_given_fake_ticket){
  //given
  Locker locker(20);
  Bag bag(666);
  (void)locker.SaveBag(bag);
  Ticket fake_ticket;

  //when
  GetBagResult result = locker.GetBag(fake_ticket);

  //then
  EXPECT_EQ(1, result.err);
  EXPECT_EQ(0, result.bag.id);
  EXPECT_EQ(19, locker.remain);
}

TEST(primary_locker_robot, should_return_ticket_when_store_bag_given_both_lockers_are_available){
  //given
  Locker locker1(10);
  Locker locker2(19);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag(666);

  //when
  SaveBagResult result = primary_locker_robot.SaveBag(bag);

  //then
  EXPECT_EQ(0, result.err);
  EXPECT_NE(0, result.ticket.id);
  EXPECT_EQ(9, locker1.remain);
  EXPECT_EQ(19, locker2.remain);
}

TEST(primary_locker_robot, should_return_ticket_when_store_bag_given_first_full_and_second_available){
  //given
  Locker locker1(1);
  Locker locker2(19);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag1(666);
  (void)primary_locker_robot.SaveBag(bag1);
  Bag bag2(6666);

  //when
  SaveBagResult result = primary_locker_robot.SaveBag(bag2);

  //then
  EXPECT_EQ(0, result.err);
  EXPECT_NE(0, result.ticket.id);
  EXPECT_EQ(0, locker1.remain);
  EXPECT_EQ(18, locker2.remain);
}

TEST(primary_locker_robot, should_show_store_error_when_store_bag_given_both_lockers_are_full){
  //given
  Locker locker1(1);
  Locker locker2(1);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag1(666);
  (void)primary_locker_robot.SaveBag(bag1);
  Bag bag2(6666);
  (void)primary_locker_robot.SaveBag(bag2);
  Bag bag3(66666);

  //when
  SaveBagResult result = primary_locker_robot.SaveBag(bag3);

  //then
  EXPECT_EQ(1, result.err);
  EXPECT_EQ(0, result.ticket.id);
  EXPECT_EQ(0, locker1.remain);
  EXPECT_EQ(0, locker2.remain);
}

TEST(primary_locker_robot, should_return_bag_when_get_bag_given_valid_ticket){
  //given
  Locker locker1(10);
  Locker locker2(19);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag(666);
  SaveBagResult save_bag_result = primary_locker_robot.SaveBag(bag);

  //when
  GetBagResult result = primary_locker_robot.GetBag(save_bag_result.ticket);

  //then
  EXPECT_EQ(0, result.err);
  EXPECT_EQ(666, result.bag.id);
  EXPECT_EQ(10, locker1.remain);
  EXPECT_EQ(19, locker2.remain);
}

TEST(primary_locker_robot, should_show_get_bag_error_when_get_bag_given_illegal_ticket){
  //given
  Locker locker1(10);
  Locker locker2(19);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag(666);
  (void)primary_locker_robot.SaveBag(bag);

  //when
  Ticket ticket;
  GetBagResult result = primary_locker_robot.GetBag(ticket);

  //then
  EXPECT_EQ(1, result.err);
  EXPECT_EQ(0, result.bag.id);
  EXPECT_EQ(9, locker1.remain);
  EXPECT_EQ(19, locker2.remain);
}

TEST(smart_locker_robot, should_store_bag_to_first_locker_when_save_bag_given_first_greater_than_second){
  Locker locker1(10);
  Locker locker2(8);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);

  Bag bag(666);
  SaveBagResult result = smart_locker_robot.SaveBag(bag);

  EXPECT_EQ(save_bag_success, result.err);
  EXPECT_EQ(9, locker1.remain);
  EXPECT_EQ(8, locker2.remain);
}

TEST(smart_locker_robot, should_store_bag_to_second_locker_when_save_bag_given_second_greater_than_first){
  Locker locker1(8);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);

  Bag bag(666);
  SaveBagResult result = smart_locker_robot.SaveBag(bag);

  EXPECT_EQ(save_bag_success, result.err);
  EXPECT_EQ(8, locker1.remain);
  EXPECT_EQ(9, locker2.remain);
}

TEST(smart_locker_robot, should_store_bag_to_first_locker_when_save_bag_given_both_are_equal){
  Locker locker1(10);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);

  Bag bag(666);
  SaveBagResult result = smart_locker_robot.SaveBag(bag);

  EXPECT_EQ(save_bag_success, result.err);
  EXPECT_EQ(9, locker1.remain);
  EXPECT_EQ(10, locker2.remain);
}

TEST(smart_locker_robot, should_show_invalid_message_when_save_bag_given_both_are_full){
  Locker locker1(1);
  Locker locker2(1);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);
  Bag bag1(666);
  (void)smart_locker_robot.SaveBag(bag1);
  Bag bag2(6666);
  (void)smart_locker_robot.SaveBag(bag2);

  Bag bag3(66);
  SaveBagResult result = smart_locker_robot.SaveBag(bag3);


  EXPECT_EQ(save_bag_locker_full, result.err);
  EXPECT_EQ(0, locker1.remain);
  EXPECT_EQ(0, locker2.remain);
}

TEST(smart_locker_robot, should_return_bag_when_smart_robot_get_bag_given_valid_ticket){
  Locker locker1(8);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);
  Bag bag(666);
  SaveBagResult save_bag_result = smart_locker_robot.SaveBag(bag);

  GetBagResult result = smart_locker_robot.GetBag(save_bag_result.ticket);

  EXPECT_EQ(get_bag_success, result.err);
  EXPECT_EQ(8, locker1.remain);
  EXPECT_EQ(10, locker2.remain);
  EXPECT_EQ(666, result.bag.id);
}

TEST(smart_locker_robot, should_show_get_bag_error_when_smart_robot_get_bag_given_illegal_ticket){
  Locker locker1(8);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);
  Bag bag(666);
  SaveBagResult save_bag_result = smart_locker_robot.SaveBag(bag);

  Ticket ticket;
  GetBagResult result = smart_locker_robot.GetBag(ticket);

  EXPECT_EQ(get_bag_illegal_ticket, result.err);
  EXPECT_EQ(8, locker1.remain);
  EXPECT_EQ(9, locker2.remain);
}

TEST(smart_locker_robot, should_return_bag_when_primary_robot_get_bag_given_ticket_from_smart_robot){
  Locker locker1(8);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag(666);
  SaveBagResult save_bag_result = smart_locker_robot.SaveBag(bag);

  GetBagResult result = primary_locker_robot.GetBag(save_bag_result.ticket);

  EXPECT_EQ(get_bag_success, result.err);
  EXPECT_EQ(8, locker1.remain);
  EXPECT_EQ(10, locker2.remain);
  EXPECT_EQ(666, result.bag.id);
}

TEST(smart_locker_robot, should_rerurn_bag_when_smart_robot_get_bag_given_ticket_from_primary_robot){
  Locker locker1(8);
  Locker locker2(10);
  std::vector<Locker*> robot_manage_lockers;
  robot_manage_lockers.push_back(&locker1);
  robot_manage_lockers.push_back(&locker2);
  SmartLockerRobot smart_locker_robot(robot_manage_lockers);
  PrimaryLockerRobot primary_locker_robot(robot_manage_lockers);
  Bag bag(666);
  SaveBagResult save_bag_result = primary_locker_robot.SaveBag(bag);

  GetBagResult result = smart_locker_robot.GetBag(save_bag_result.ticket);

  EXPECT_EQ(get_bag_success, result.err);
  EXPECT_EQ(8, locker1.remain);
  EXPECT_EQ(10, locker2.remain);
  EXPECT_EQ(666, result.bag.id);
}





