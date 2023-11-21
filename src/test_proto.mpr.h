#pragma once
#include <elastic.hpp>

/****************
*test_proto
*****************/
namespace xxx // package named namespace
{
	//message person that number is 10001
	class person final : public elastic::message_lite<person>
	{
	public:
		person() = default;
		virtual ~person() = default;
		
	private:
		virtual bool internal_from_binary(elastic::flex_buffer_t& buffer) final;
		
		virtual bool internal_to_binary(elastic::flex_buffer_t& buffer) final;
		
	public:
		int32 age;		//����
		string name;	//����
		bool sex;
		int64 money;	//Ǯ
		uint32 back_money; //�ֿ����Ǯ
		uint64 crc; //crc��֤��
		bytes role_data; //��������
		float hp; //����Ѫ��
		double mana; //��������
		fixed32 input_stream; //������
		fixed64 output_stream; //�����
	};
}
