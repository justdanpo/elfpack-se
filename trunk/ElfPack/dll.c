#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\dll.h"
#include "vars.h"
#include "elfloader.h"

int CompareDllListElements(DLL_LIST_ELEMENT* listitem, wchar_t* itemtofind)
{
	return wstrcmpi(listitem->name, itemtofind);
}

// ���������� ����� this ��� errorcode
void * LoadDLL_int(wchar_t * DllName)
{
	DLL_LIST_ELEMENT * dll;
	DLL_DATA * dll_public_data;

	int index = LIST_FIND(elfpackdata->DLLList, DllName, CompareDllListElements);
	if (index==LIST_ERROR)
	{
		//���� �� ��������� DLL��
		if (fstat(GetDir(DIR_DLL | MEM_INTERNAL),DllName,0))  return(DLL_ERROR_FILE_NOT_FOUND);
		wchar_t * dllfullname = malloc( (wstrlen( GetDir(DIR_DLL | MEM_INTERNAL))*2)+ sizeof(L"/")+ (wstrlen(DllName)*2)+2);
		DLL_LIST_ELEMENT * newdll = malloc(sizeof(DLL_LIST_ELEMENT));
		wchar_t * name = malloc((wstrlen(DllName)*2)+2);
		// ������� ����
		wstrcpy(dllfullname,GetDir(DIR_DLL | MEM_INTERNAL));
		wstrcat(dllfullname,L"/");
		wstrcat(dllfullname,DllName);
		//������� ����� ���
		wstrcpy(name,DllName);
		newdll->name=name;

		newdll->EntryPoint = (int (*)(int,DLL_DATA*)) elfload_int(dllfullname,0,0,0);
		mfree(dllfullname);

		// ���� �� �������� EntryPoint
		if ((int)newdll->EntryPoint<=0)
		{
			mfree(newdll->name);
			mfree(newdll);
			return(DLL_ERROR_FILE_NOT_LOADED);
		}
		IMB();
		// ���� �� ������������ - �����
		if (newdll->EntryPoint(DLL_INIT,0))
		{
			mfree(newdll->name);
			mfree(newdll);
			return(DLL_ERROR_INIT);
		}

		// ���� DLL �� ������ ����� this
		if (!(dll_public_data=(void*)newdll->EntryPoint(DLL_LOAD,0)))
		{
			mfree(newdll->name);
			mfree(newdll);
			return(DLL_ERROR_LOAD);
		}

		// �������� � ������ ����������� DLL
		List_InsertLast(elfpackdata->DLLList, newdll);
		// ��� DLL ������ � this
		dll_public_data->name =  malloc((wstrlen(DllName)*2)+2);
		wstrcpy(dll_public_data->name,DllName);
		// ������� ����� this
		return(dll_public_data);
	}
	else
	{
		// ���� ����� DLL ����������, �� ����� �� ������
		dll = List_Get(elfpackdata->DLLList, index);
	}

	// ���� DLL �� ������ ����� this
	if (!(dll_public_data=(void*)dll->EntryPoint(DLL_LOAD,0)))
	{
		return(DLL_ERROR_LOAD);
	}

	// ��� DLL ������ � this
	dll_public_data->name = malloc((wstrlen(DllName)*2)+2);
	wstrcpy(dll_public_data->name,DllName);

	// ������� ����� this
	return(dll_public_data);
}


int UnLoadDLL_int(DLL_DATA * DllData)
{
	DLL_LIST_ELEMENT * dll;
	int usage_count;

	// ���� ���� this
	if (!DllData)    return(DLL_ERROR_NO_DATA_P);

	// ���� � ������ DLL
	int index = LIST_FIND(elfpackdata->DLLList, DllData->name, CompareDllListElements);
	if (index==LIST_ERROR) return(DLL_ERROR_DLL_NOT_LOADED);

	dll = List_Get(elfpackdata->DLLList, index);

	// �������� � UnLoader
	mfree(DllData->name);
	usage_count=dll->EntryPoint(DLL_UNLOAD,DllData);

	// ���� ������������ ��� ��� ��
	if (usage_count) return(usage_count);

	// ����� ������� ���,������� �� ������ � ������� ��� DLL_LIST_ELEMENT
	mfree(dll->name);
	mfree(List_RemoveAt(elfpackdata->DLLList, index));
	return(0);
}
