#pragma once
#include <elastic.hpp>

/****************
*test_proto
*****************/
namespace xxx // package named namespace
{
	//message person that number is 10001
	class person final : public elastic::message_lite<person> // 注释
	{ //begin
		friend class elastic::message_lite<person>;
		struct member_impl : elastic::message_pod
		{
			REFLECT_DEFINE
			(
				int32_t age;		//年龄
				std::string name;	//姓名
				bool sex;
				int64_t money;	//钱
				uint32_t back_money; //仓库里的钱
				uint64_t crc; //crc验证码
				bytes role_data; //人物数据
				float hp; //人物血量
				double mana; //人物蓝量
				fixed32_t input_stream; //输入流
				fixed64_t output_stream; //输出流
				std::map<uint32_t,std::string> pairs;
			)
			
			bool operator==(const member_impl& other) const
			{
				return
					age == other.age &&
					name == other.name &&
					sex == other.sex &&
					money == other.money &&
					back_money == other.back_money &&
					crc == other.crc &&
					role_data == other.role_data &&
					hp == other.hp &&
					mana == other.mana &&
					input_stream == other.input_stream &&
					output_stream == other.output_stream &&
					pairs == other.pairs;
			}
			private:
				friend class elastic::access;
			
				template<typename _Archive>
				void serialize(_Archive& ar)
				{
					ar& age;
					ar& name;
					ar& sex;
					ar& money;
					ar& back_money;
					ar& crc;
					ar& role_data;
					ar& hp;
					ar& mana;
					ar& input_stream;
					ar& output_stream;
					ar& pairs;
				}
		};
		
		public:
			using pod_t = member_impl;
		
		public:
			person() = default;
			virtual ~person() = default;
		
		public:
			bool operator==(const person& other)
			{
				return impl == other.impl;
			}
		
		public:
			int32_t age() const;

			void set_age(const int32_t& age);

			void clear_age();

			std::string name() const;

			void set_name(const std::string& name);

			void clear_name();

			bool sex() const;

			void set_sex(const bool& sex);

			void clear_sex();

			int64_t money() const;

			void set_money(const int64_t& money);

			void clear_money();

			uint32_t back_money() const;

			void set_back_money(const uint32_t& back_money);

			void clear_back_money();

			uint64_t crc() const;

			void set_crc(const uint64_t& crc);

			void clear_crc();

			bytes role_data() const;

			void set_role_data(const bytes& role_data);

			void clear_role_data();

			float hp() const;

			void set_hp(const float& hp);

			void clear_hp();

			double mana() const;

			void set_mana(const double& mana);

			void clear_mana();

			fixed32_t input_stream() const;

			void set_input_stream(const fixed32_t& input_stream);

			void clear_input_stream();

			fixed64_t output_stream() const;

			void set_output_stream(const fixed64_t& output_stream);

			void clear_output_stream();

			std::map<uint32_t,std::string> pairs() const;

			void set_pairs(const std::map<uint32_t,std::string>& pairs);

			void clear_pairs();

		public:
			virtual elastic::message_pod& internal_type() final;

		private:
			member_impl impl;
	};//end
}
