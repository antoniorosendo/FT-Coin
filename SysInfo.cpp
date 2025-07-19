#include "SysInfo.hpp"

const string SysInfo::author = "Andre F. de Angelis";
const string SysInfo::date = "2025, June";
const string SysInfo::department = "Faculdade de Tecnologia - Limeira/SP";
const string SysInfo::institution = "Universidade Estadual de Campinas (Unicamp)";
const string SysInfo::systemName = "FT Coin";
const string SysInfo::version = "0.10";
const string SysInfo::helpFile = "txt/FTC_Help.txt";

const string& SysInfo::getAuthor()
	{
	return author;
	}

const string& SysInfo::getDate()
	{
	return date;
	}

const string& SysInfo::getDepartment()
	{
	return department;
	}

const string& SysInfo::getInstitution()
	{
	return institution;
	}

const string& SysInfo::getSystemName()
	{
	return systemName;
	}

const string& SysInfo::getVersion()
	{
	return version;
	}

const string& SysInfo::getHelpFile()
	{
	return helpFile;
	}

const string SysInfo::getFullVersion()
	{
	return systemName + " | Ver. " + version + " (" + date + ")";
	}
