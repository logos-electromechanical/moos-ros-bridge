#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

#include <chrono>

using namespace std::chrono_literals;

class Counter : public rclcpp::Node
{
     public:
          Counter() : Node("counter"), count(0)
          {
               count = 0;
               pub = this->create_publisher<std_msgs::msg::Int32>("ros_counter", 10);
               auto timer_cb =
                    [this]() -> void {
                         auto msg = std_msgs::msg::Int32();
                         msg.data = count;
                         this->pub->publish(msg);
                         count += 1;
                    };
               timer = this->create_wall_timer(1000ms, timer_cb);
          }

     private:
          rclcpp::TimerBase::SharedPtr timer;
          rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub;
          int32_t count;

};

int main(int argc, char **argv)
{
     rclcpp::init(argc, argv);
     rclcpp::spin(std::make_shared<Counter>());
     rclcpp::shutdown();

     return 0;
}
