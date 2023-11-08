#pragma once
#include <elastic.hpp>

/****************
*test_proto
*****************/
namespace xxx // package named namespace
{
	//message person that number is 10001
	class person final : public elastic::message_lite<person> // ע��
	{
		friend class elastic::message_lite<person>;

		struct member_impl : elastic::message_pod
		{
		REFLECT_DEFINE(//begin

			int32_t age;		//����

			std::string name;	//����

			bool sex;

			int64_t money;	//Ǯ

			uint32_t back_money; //�ֿ����Ǯ

			uint64_t crc; //crc��֤��

			std::vector<uint8_t> role_data; //��������

			float hp; //����Ѫ��

			double mana; //��������

			elastic::fixed<uint32_t> input_stream; //������

			elastic::fixed<uint64_t> output_stream; //�����
		)
		};

	public :
		using pod_t = member_impl;

	public:
		person() =default;

		virtual ~person() = default;

	public:
		int32_t age();

		int32_t age() const;

		void set_age(const int32_t& age);

		const std::string& name();

		const std::string& name(); const

		void set_name(const std::string& name);

		const bool& sex();

		const bool& sex(); const

		void set_sex(const bool& sex);

		const int64_t& money();

		const int64_t& money(); const

		void set_money(const int64_t& money);

		const uint32_t& back_money();

		const uint32_t& back_money(); const

		void set_back_money(const uint32_t& back_money);

		const uint64_t& crc();

		const uint64_t& crc(); const

		void set_crc(const uint64_t& crc);

		const std::vector<uint8_t>& role_data();

		const std::vector<uint8_t>& role_data(); const

		void set_role_data(const std::vector<uint8_t>& role_data);

		const float& hp();

		const float& hp(); const

		void set_hp(const float& hp);

		const double& mana();

		const double& mana(); const

		void set_mana(const double& mana);

		const elastic::fixed<uint32_t>& input_stream();

		const elastic::fixed<uint32_t>& input_stream(); const

		void set_input_stream(const elastic::fixed<uint32_t>& input_stream);

		const elastic::fixed<uint64_t>& output_stream();

		const elastic::fixed<uint64_t>& output_stream(); const

		void set_output_stream(const elastic::fixed<uint64_t>& output_stream);

	private:
		virtual elastic::message_pod& internal_type() final;

 	private:
		member_impl impl;
	};	//end
}