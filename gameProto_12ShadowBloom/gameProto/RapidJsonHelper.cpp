#include "RapidJsonHelper.h"

//////////////////////////////////////////////////////////////////////////
// json�t�@�C���̃I�[�v�� 
// in    : inFileName : �I�[�v��������json�t�@�C���p�X
// inout : inoutDoc   : rapidjson �I�u�W�F�N�g
// �I�[�v���ł������� true��Ԃ��AinoutDoc�Ɂ@rapidjsonDocument���Q�Ƃł���
//////////////////////////////////////////////////////////////////////////
bool openJsonFile(rapidjson::Document & inoutDoc, const char * inFileName)
{
	// json�t�@�C���J���邩�H
	std::ifstream file(inFileName);
	if (!file.is_open())
	{
		return false;
	}

	// filestream�Ƀt�@�C���������荞��
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// �t�@�C���������JSon������ɕϊ�
	rapidjson::StringStream jsonStr(contents.c_str());
	// �t�@�C���X�g���[������
	file.close();

	// ��̓I�u�W�F�N�g�쐬
	inoutDoc.ParseStream(jsonStr);

	// JSonObject�Ƃ��ēǂ߂����H
	if (!inoutDoc.IsObject())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Doc���ɂ��̃����o�[�͑��݂��邩�H
// in : inDoc      rapidjson Document
//    : memberName �����o�[��
// �߂�l�@Document���� memberName�����݂����Ƃ��̂�true��Ԃ��B
///////////////////////////////////////////////////////////////////////////////
bool IsExistMember(const rapidjson::Document & inDoc, const char * memberName)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = inDoc.FindMember(memberName);

	if (iter == inDoc.MemberEnd())
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
// ���̃����o�[�����݂��A���l����v���邩�H
// in    :  inDoc       rapidjson�h�L�������g
//          memberName  ���ׂ��������o�[��
//          matchValue   ���ׂ��������o�[���Ƀ}�b�`����l
// �߂�l : memberName�ɑΉ�����l��matchValue�������Ƃ��̂�true��Ԃ� 
/////////////////////////////////////////////////////////////////////////////////
bool IsExistMemberAndValue(const rapidjson::Document & inDoc, const char * memberName, const char * matchValue)
{
	if (!IsExistMember(inDoc, memberName))
	{
		return false;
	}

	std::string findValueString;
	findValueString = std::string(matchValue);

	if (findValueString != inDoc[memberName].GetString())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
// Doc���� memberName�Ƃ������O�̔z��͂��邩�H
//////////////////////////////////////////////////////////////////////////
bool IsExistArrayName(const rapidjson::Document & inDoc, const char * memberName)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = inDoc.FindMember(memberName);
	if (iter == inDoc.MemberEnd())
	{
		return false;
	}
	return inDoc[memberName].IsArray();
}

// JSON�l����e��int�ł������I��float �l�Ƃ��Ď擾
float ForceGetFloat(const rapidjson::Value& val)
{
	std::string tmp;
	tmp = val.GetString();

	return static_cast<float>(atof(tmp.c_str()));
}
