#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int64.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"

#include <chrono>
#include <string>

using namespace std::chrono_literals;

class Counter : public rclcpp::Node
{
     public:
          Counter() : Node("counter"), count(0)
          {
               count = 0;
               pub32 = this->create_publisher<std_msgs::msg::Int32>("count32", 10);
               pub64 = this->create_publisher<std_msgs::msg::Int64>("count64", 10);
               pubf32 = this->create_publisher<std_msgs::msg::Float32>("countf32", 10);
               pubf64 = this->create_publisher<std_msgs::msg::Float64>("countf64", 10);
               pubstr = this->create_publisher<std_msgs::msg::String>("countstr", 10);
               auto timer_cb =
                    [this]() -> void {
                         auto msg32 = std_msgs::msg::Int32();
                         auto msg64 = std_msgs::msg::Int64();
                         auto msgf32 = std_msgs::msg::Float32();
                         auto msgf64 = std_msgs::msg::Float64();
                         auto msgstr = std_msgs::msg::String();
                         msg32.data = count;
                         msg64.data = count;
                         msgf32.data = count;
                         msgf64.data = count;
                         msgstr.data = std::to_string(count) + " ff\n";
                         this->pub32->publish(msg32);
                         this->pub64->publish(msg64);
                         this->pubf32->publish(msgf32);
                         this->pubf64->publish(msgf64);
                         this->pubstr->publish(msgstr);
                         count += 2;
                    };
               timer = this->create_wall_timer(1000ms, timer_cb);
          }

     private:
          rclcpp::TimerBase::SharedPtr timer;
          rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub32;
          rclcpp::Publisher<std_msgs::msg::Int64>::SharedPtr pub64;
          rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pubf32;
          rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pubf64;
          rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubstr;
          int32_t count;

};

int main(int argc, char **argv)
{
     rclcpp::init(argc, argv);
     rclcpp::spin(std::make_shared<Counter>());
     rclcpp::shutdown();

     return 0;
}
