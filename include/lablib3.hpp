#pragma once

#include "lablib3.h"

#include <exception>
#include <vector>
#include <string>
#include <sstream>

namespace Odl
{
	class Keyword
	{
		friend class ObjDesc;

	public:
		Keyword() : m_kwd(), m_free_on_delete(false){}

		Keyword(KEYWORD *kwd) :
			m_kwd(kwd),
			m_free_on_delete(false)
		{
		}

		Keyword(const std::string &keyword_name, const char* value_text, const char* pre_comment, const char* line_comment, const char* file_name, long line_number) :
			m_free_on_delete(false)
		{
			m_kwd = OdlNewKwd((char *)keyword_name.c_str(), (char *)value_text, (char *)pre_comment, (char *)line_comment, (char *)file_name, line_number);
			if (!m_kwd)
			{
				throw std::runtime_error("Failed to create new keyword");
			}

			m_free_on_delete = true;
		}

		Keyword(const std::string &keyword_name, long long value, const char* pre_comment, const char* line_comment, const char* file_name, long line_number) :
			m_free_on_delete(false)
		{
			std::ostringstream value_text;

			value_text << value;

			m_kwd = OdlNewKwd((char *)keyword_name.c_str(), (char *)value_text.str().c_str(), (char *)pre_comment, (char *)line_comment, (char *)file_name, line_number);
			if (!m_kwd)
			{
				throw std::runtime_error("Failed to create new keyword");
			}

			m_free_on_delete = true;
		}

		Keyword(const Keyword &kwd) :
			m_free_on_delete(false)
		{
			m_kwd = OdlCopyKwd(kwd.m_kwd);
			if (!m_kwd)
			{
				throw std::runtime_error("Failed to copy keyword");
			}

			m_free_on_delete = true;
		}

		~Keyword()
		{
			if (m_free_on_delete)
			{
				OdlFreeKwd(m_kwd);
			}
		}

		int			GetInt() const
		{
			return atoi(OdlGetKwdValue(m_kwd));
		}

		double		GetDouble() const
		{
			char *	end = nullptr;
			return strtod(OdlGetKwdValue(m_kwd), &end);
		}

		auto		GetAllValues() const
		{
			TB_STRING_LIST *		value_list = OdlGetAllKwdValues(m_kwd);
			std::vector<std::string>	cpp_list;

			for (TB_STRING_LIST *l = value_list; l != nullptr; l = l->next)
			{
				cpp_list.push_back(l->text);
			}

			RemoveStringList(value_list);
			return cpp_list;
		}

		KEYWORD *	operator()() const { return m_kwd; }
		operator KEYWORD* () const { return m_kwd; }

	private:
		KEYWORD *	m_kwd;
		mutable bool	m_free_on_delete;

		void Detach() const { m_free_on_delete = false; }
	};

	class ObjDesc
	{
	public:
		ObjDesc(OBJDESC *desc, bool free_on_delete) :
			m_root(desc),
			m_free_on_delete(free_on_delete)
		{
		}

		ObjDesc(const std::string &object_class, const char* pre_comment, const char* line_comment, const char* post_comment, const char* end_comment, const char* file_name, short is_a_group, long line_number) :
			m_free_on_delete(false)
		{
			m_root = OdlNewObjDesc((char *)object_class.c_str(), (char *)pre_comment, (char *)line_comment, (char *)post_comment, (char *)end_comment, (char *)file_name, is_a_group, line_number);
			if (!m_root)
			{
				throw std::runtime_error("Failed to create new OBJDESC");
			}

			m_free_on_delete = true;
		}

		~ObjDesc()
		{
			if (m_free_on_delete)
			{
				OdlFreeTree(m_root);
			}
		}

		Keyword		FindKwd(const std::string &keyword_name, const char* keyword_value, unsigned long keyword_position, unsigned short search_scope) const
		{
			KEYWORD *	kwd = OdlFindKwd(m_root, (char *)keyword_name.c_str(), (char *)keyword_value, keyword_position, search_scope);

			if (!kwd)
			{
				throw std::runtime_error("Failed to find keyword");
			}

			return Keyword(kwd);
		}

		ObjDesc		FindObjDesc(const std::string &object_class, const char* keyword_name, const char* keyword_value, unsigned long object_position, unsigned short search_scope) const
		{
			OBJDESC *	desc = OdlFindObjDesc(m_root, (char *)object_class.c_str(), (char *)keyword_name, (char *)keyword_value, object_position, search_scope);

			if (!desc)
			{
				throw std::runtime_error("Failed to find object");
			}

			return ObjDesc(desc, false);
		}

		void PasteKwd(const Keyword &kwd)
		{
			OdlPasteKwd(kwd(), m_root);

			kwd.Detach();
		}

		void PasteObjDesc(const ObjDesc &child)
		{
			OdlPasteObjDesc(child.m_root, m_root);
			child.Detach();
		}

	protected:
		OBJDESC *	m_root;
		mutable bool		m_free_on_delete;

		void Detach() const { m_free_on_delete = false; }
	};

	class InputLabel : public ObjDesc
	{
	public:
		InputLabel() :
			ObjDesc(NULL, false)
		{
		}

		InputLabel(const std::string &path, const char* log_path, MASK expand, unsigned short suppress_messages) :
			ObjDesc(NULL, true)
		{
			Open(path, log_path, expand, suppress_messages);
		}

		void Open(const std::string &path, const char* log_path, MASK expand, unsigned short suppress_messages)
		{
			m_root = OdlParseLabelFile((char *)path.c_str(), (char *)log_path, expand, suppress_messages);
			if (!m_root)
			{
				throw std::runtime_error("Failed to parse label file");
			}

			m_free_on_delete = true;
		}
	};

	class OutputLabel : public ObjDesc
	{
	public:
		OutputLabel() :
			ObjDesc(NULL, false)
		{
		}

		OutputLabel(const std::string &object_class, const char* pre_comment, const char* line_comment, const char* post_comment, const char* end_comment, const char* file_name, short is_a_group, long line_number) :
			ObjDesc(NULL, true)
		{
			Create(object_class, pre_comment, line_comment, post_comment, end_comment, file_name, is_a_group, line_number);
		}

		void Create(const std::string &object_class, const char* pre_comment, const char* line_comment, const char* post_comment, const char* end_comment, const char* file_name, short is_a_group, long line_number)
		{
			m_root = OdlNewObjDesc((char *)object_class.c_str(), (char *)pre_comment, (char *)line_comment, (char *)post_comment, (char *)end_comment, (char *)file_name, is_a_group, line_number);
			if (!m_root)
			{
				throw std::runtime_error("Failed to create label root");
			}

			m_free_on_delete = true;
		}

		void PrintLabel(const char *fname, FILE *fptr, unsigned long root_level, MASK options)
		{
			OdlPrintLabel(m_root, (char *)fname, fptr, root_level, options);
		}
	};
}