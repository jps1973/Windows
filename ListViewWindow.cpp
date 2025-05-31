// ListViewWindow.cpp

#include "ListViewWindow.h"

// Global variables
static HWND g_hWndListView;

BOOL IsListViewWindow( HWND hWnd )
{
	// See if supplied window is list view window
	return( hWnd == g_hWndListView );

} // End of function IsListViewWindow

int ListViewWindowAddItem( LPCTSTR lpszItem, DWORD dwMaximumTextLength )
{
	int nResult = 0;

	LVITEM lvItem;
	int nItemCount;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Count items on list view window
	nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nItemCount;
	lvItem.iSubItem		= 0;
	lvItem.pszText		= ( LPTSTR )lpszItem;

	// Add item to list view window
	SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

	return nResult;

} // End of function ListViewWindowAddItem

int ListViewWindowAutoSizeAllColumns()
{
	int nResult = 0;

	int nWhichColumn;

	// Loop through all columns
	for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
	{
		// Auto-size current column
		if( SendMessage( g_hWndListView, LVM_SETCOLUMNWIDTH, ( WPARAM )nWhichColumn, ( LPARAM )LVSCW_AUTOSIZE_USEHEADER ) )
		{
			// Successfully auto-sized current column

			// Update return value
			nResult ++;

		} // End of successfully auto-sized current column
		else
		{
			// Unable to auto-size current column

			// Force exit from loop
			nWhichColumn = LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS;

		} // End of unable to auto-size current column

	}; // End of loop through all columns

	return nResult;

} // End of function ListViewWindowAutoSizeAllColumns

int CALLBACK ListViewWindowCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lParamColumn )
{
	int nResult = 0;

	LVITEM lvItem;

	// Allocate string memory
	LPTSTR lpszItem1 = new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszItem2 = new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.iSubItem		= lParamColumn;
	lvItem.cchTextMax	= STRING_LENGTH;

	// Update list view item structure for first item
	lvItem.iItem	= lParam1;
	lvItem.pszText	= lpszItem1;

	// Get first item text
	if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam1, ( LPARAM )&lvItem ) )
	{
		// Successfully got first item text

		// Update list view item structure for second item
		lvItem.iItem	= lParam2;
		lvItem.pszText	= lpszItem2;

		// Get second item text
		if( SendMessage( g_hWndListView, LVM_GETITEMTEXT, ( WPARAM )lParam2, ( LPARAM )&lvItem ) )
		{
			// Successfully got second item text

			// Compare item texts
			nResult = lstrcmp( lpszItem1, lpszItem2 );

		} // End of successfully got second item text

	} // End of successfully got first item text

	// Free string memory
	delete [] lpszItem1;
	delete [] lpszItem2;

	return nResult;

} // End of function ListViewWindowCompare

BOOL ListViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list view window
	g_hWndListView = CreateWindowEx( LIST_VIEW_WINDOW_EXTENDED_STYLE, LIST_VIEW_WINDOW_CLASS_NAME, LIST_VIEW_WINDOW_TEXT, LIST_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list view window was created
	if( g_hWndListView )
	{
		// Successfully created list view window
		int nWhichColumn;
		LVCOLUMN lvColumn;
		LPCTSTR lpszColumnTitles [] = LIST_VIEW_WINDOW_COLUMN_TITLES;

		// Clear list view column structure
		ZeroMemory( &lvColumn, sizeof( lvColumn ) );

		// Initialise list view column structure
		lvColumn.mask = LVCF_TEXT;

		// Set extended list view window style
		SendMessage( g_hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, ( WPARAM )NULL, ( LPARAM )LIST_VIEW_WINDOW_EXTENDED_STYLE );

		// Loop through all list view window columns
		for( nWhichColumn = 0; nWhichColumn < LIST_VIEW_WINDOW_NUMBER_OF_COLUMNS; nWhichColumn ++ )
		{
			// Update list view column structure for current column
			lvColumn.pszText = ( LPTSTR )lpszColumnTitles[ nWhichColumn ];

			// Insert current column
			SendMessage( g_hWndListView, LVM_INSERTCOLUMN, ( WPARAM )nWhichColumn, ( LPARAM )&lvColumn );

		}; // End of loop through all list view window columns

		// Auto-size all list view window columns
		ListViewWindowAutoSizeAllColumns();

		// Update return value
		bResult = TRUE;

	} // End of successfully created list view window
	return bResult;

} // End of function ListViewWindowCreate

int ListViewWindowGetColumnCount()
{
	int nResult = 0;

	HWND hWndListViewHeaderControl;

	// Get list view header control window
	hWndListViewHeaderControl = ( HWND )SendMessage( g_hWndListView, LVM_GETHEADER, ( WPARAM )NULL, ( LPARAM )NULL );

	// Ensure that list view header control window was got
	if( hWndListViewHeaderControl )
	{
		// Successfully got list view header control window

		// Count items in list view header control window
		nResult = ( int )SendMessage( hWndListViewHeaderControl, HDM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

	} // End of successfully got list view header control window

	return nResult;

} // End of function ListViewWindowGetColumnCount

BOOL ListViewWindowGetItemText( int nWhichItem, int nWhichSubItem, LPTSTR lpszItemText, DWORD dwMaximumTextLength )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= lpszItemText;

	// Get item from list view window
	bResult = SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowGetItemText

BOOL ListViewWindowGetRect( LPRECT lpRect )
{
	// Get list view window rect
	return GetWindowRect( g_hWndListView, lpRect );

} // End of function ListViewWindowGetRect

BOOL ListViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMLISTVIEW lpNmListView;

	// Get list view notify message handler
	lpNmListView = ( LPNMLISTVIEW )lParam;

	// Select list view window notification code
	switch( lpNmListView->hdr.code )
	{
		case NM_DBLCLK:
		{
			// A double click notify message

			// Allocate string memory
			LPTSTR lpszWindowText = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get window text
			if( ListViewWindowGetItemText( lpNmListView->iItem, LIST_VIEW_WINDOW_TEXT_COLUMN_ID, lpszWindowText ) )
			{
				// Successfully got window text

				// Allocate string memory
				LPTSTR lpszWindowClassName = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get window class name
				if( ListViewWindowGetItemText( lpNmListView->iItem, LIST_VIEW_WINDOW_CLASS_NAME_COLUMN_ID, lpszWindowClassName ) )
				{
					// Successfully got window class name

					// Allocate string memory
					LPTSTR lpszWarningMessage = new char[ STRING_LENGTH + sizeof( char ) ];

					// Format warning text
					wsprintf( lpszWarningMessage, LIST_VIEW_WINDOW_CLOSE_WINDOW_WARNING_MESSAGE_FORMAT_STRING, lpszWindowText, lpszWindowClassName );

					// Ensure that user wants to close window
					if( MessageBox( NULL, lpszWarningMessage, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING ) ) == IDYES )
					{
						// User wants to close window
						HWND hWnd;

						// Find window
						hWnd = FindWindow( lpszWindowClassName, lpszWindowText );

						// Ensure that window was found
						if( hWnd )
						{
							// Successfully found window

							// Close window
							SendMessage( hWnd, WM_CLOSE, ( WPARAM )NULL, ( LPARAM )NULL );

							// Remove window from list view window
							SendMessage( g_hWndListView, LVM_DELETEITEM, ( WPARAM )lpNmListView->iItem, ( LPARAM )NULL );

						} // End of successfully found window

					} // End of user wants to close window

					// Free string memory
					delete [] lpszWarningMessage;

				} // End of successfully got window class name

				// Free string memory
				delete [] lpszWindowClassName;

			} // End of successfully got window text

			// Free string memory
			delete [] lpszWindowText;

			// Break out of switch
			break;

		} // End of a double click notify message
		case LVN_COLUMNCLICK:
		{
			// A column click notify message

			// Sort the list view
			ListView_SortItemsEx( g_hWndListView, &ListViewWindowCompare, lpNmListView->iSubItem );

			// Break out of switch
			break;

		} // End of a column click notify message
		case LVN_ITEMCHANGED:
		{
			// A list view item changed notify message

			// See if item state has changed to selected
			if( ( lpNmListView->uNewState ^ lpNmListView->uOldState ) & LVIS_SELECTED )
			{
				// Item state has changed to selected

				// Allocate string memory
				LPTSTR lpszWindowText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get window text
				if( ListViewWindowGetItemText( lpNmListView->iItem, LIST_VIEW_WINDOW_TEXT_COLUMN_ID, lpszWindowText ) )
				{
					// Successfully got window text

					// Call status function
					bResult = ( *lpStatusFunction )( lpszWindowText );

				} // End of successfully got window text

				// Free string memory
				delete [] lpszWindowText;

			} // End of item state has changed to selected

			// Break out of switch
			break;

		} // End of a list view item changed notify message
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list view window notification code

	return bResult;

} // End of function ListViewWindowHandleNotifyMessage

BOOL ListViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list view window
	return MoveWindow( g_hWndListView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListViewWindowMove

int ListViewWindowLoad( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPTSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszLine;
				LVITEM lvItem;
				int nItemCount;
				int nInserted;

				// Clear list view item structure
				ZeroMemory( &lvItem, sizeof( lvItem ) );

				// Count items on list view window
				nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

				// Initialise list view item structure
				lvItem.mask			= LVIF_TEXT;
				lvItem.cchTextMax	= STRING_LENGTH;
				lvItem.iItem		= nItemCount;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line in file text
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all lines in file text
				while( lpszLine )
				{
					// Update list view item structure for line
					lvItem.iSubItem	= LIST_VIEW_WINDOW_TEXT_COLUMN_ID;
					lvItem.pszText	= lpszLine;

					// Add line to list view window
					nInserted = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )nItemCount, ( LPARAM )&lvItem );

					// Ensure that line was added to list view window
					if( nInserted >= 0 )
					{
						// Successfully added line to list view window

						// Update list view item structure
						lvItem.iItem = nInserted;

						// Update return value
						nResult ++;

						// Advance to next item
						lvItem.iItem ++;

						// Get next line in file text
						lpszLine = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added line to list view window
					else
					{
						// Unable to add line to list view window

						// Force exit from loop
						lpszLine = NULL;

					} // End of unable to add line to list view window

				}; // End of loop through all lines in file text

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function ListViewWindowLoad

int ListViewWindowPopulate()
{
	int nResult = 0;

	HWND hWndCurrent;
	LVITEM lvItem;
	int nInserted;

	// Allocate string memory
	LPTSTR lpszWindowText		= new char[ STRING_LENGTH + sizeof( char ) ];
	LPTSTR lpszWindowClassName	= new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= STRING_LENGTH;
	lvItem.iItem		= 0;

	// Clear list view window
	SendMessage( g_hWndListView, LB_RESETCONTENT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Get top window
	hWndCurrent = GetTopWindow( NULL );

	// Loop through all windows
	while( hWndCurrent )
	{
		// Get current window text
		GetWindowText( hWndCurrent, lpszWindowText, STRING_LENGTH );

		// Update list view item structure for current window text
		lvItem.iSubItem	= LIST_VIEW_WINDOW_TEXT_COLUMN_ID;
		lvItem.pszText	= lpszWindowText;

		// Add current window text to list view window
		nInserted = SendMessage( g_hWndListView, LVM_INSERTITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

		// Ensure that current window text was added to list view window
		if( nInserted >= 0 )
		{
			// Successfully added current window text to list view window

			// Update list view item structure
			lvItem.iItem = nInserted;

			// Get current window class name
			GetClassName( hWndCurrent, lpszWindowClassName, STRING_LENGTH );

			// Update list view item structure for current window class name
			lvItem.iSubItem	= LIST_VIEW_WINDOW_CLASS_NAME_COLUMN_ID;
			lvItem.pszText	= lpszWindowClassName;

			// Show current window class name
			SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )lvItem.iItem, ( LPARAM )&lvItem );

			// Update return value
			nResult ++;

			// Advance to next item
			lvItem.iItem ++;

		} // End of successfully added current window text to list view window
		else
		{
			// Unable to add current window text to list view window

			// Force exit from loop
			lpszWindowText = NULL;

		} // End of unable to add current window text to list view window

		// Get next window
		hWndCurrent = GetNextWindow( hWndCurrent, GW_HWNDNEXT );

	}; // End of loop through all windows

	// Auto-size all list view window columns
	ListViewWindowAutoSizeAllColumns();

	// Free string memory
	delete [] lpszWindowText;
	delete [] lpszWindowClassName;

	return nResult;

} // End of function ListViewWindowPopulate

int ListViewWindowSave( LPCTSTR lpszFileName, LPCTSTR lpszItemSeparator )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		int nColumnCount;
		LVITEM lvItem;
		DWORD dwItemSeperatorLength;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Clear list view item structure
		ZeroMemory( &lvItem, sizeof( lvItem ) );

		// Initialise list view item structure
		lvItem.mask			= LVIF_TEXT;
		lvItem.cchTextMax	= STRING_LENGTH;
		lvItem.pszText		= lpszItemText;

		// Store item separator length
		dwItemSeperatorLength = lstrlen( lpszItemSeparator );

		// Count items on list view window
		nItemCount = SendMessage( g_hWndListView, LVM_GETITEMCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Count columns on list view window
		nColumnCount = ListViewWindowGetColumnCount();

		// Loop through items on list view window
		for( lvItem.iItem = 0; lvItem.iItem < nItemCount; lvItem.iItem ++ )
		{
			// Loop through columns on list view window
			for( lvItem.iSubItem = 0; lvItem.iSubItem < nColumnCount; lvItem.iSubItem ++ )
			{
				// Get item text
				if( SendMessage( g_hWndListView, LVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem ) )
				{
					// Successfully got item text

					// Write item text to file
					if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
					{
						// Successfully wrote item text to file

						// Write item separator to file
						WriteFile( hFile, lpszItemSeparator, dwItemSeperatorLength, NULL, NULL );

						// Update return value
						nResult ++;

					} // End of successfully wrote item text to file
					else
					{
						// Unable to write item text to file

						// Force exit from loops
						lvItem.iItem	= nItemCount;
						lvItem.iSubItem	= nColumnCount;

					} // End of unable to write item text to file

				} // End of successfully got item text
				else
				{
					// Unable to get item text

					// Force exit from loops
					lvItem.iItem	= nItemCount;
					lvItem.iSubItem	= nColumnCount;

				} // End of unable to get item text

			}; // End of loop through columns on list view window

			// Write new line text to file
			WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

		}; // End of loop through items on list view window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function ListViewWindowSave

HWND ListViewWindowSetFocus()
{
	// Focus on list view window
	return SetFocus( g_hWndListView );

} // End of function ListViewWindowSetFocus

BOOL ListViewWindowSetItemText( int nWhichItem, int nWhichSubItem, LPCTSTR lpszItemText, DWORD dwMaximumTextLength )
{
	BOOL bResult = FALSE;

	LVITEM lvItem;

	// Clear list view item structure
	ZeroMemory( &lvItem, sizeof( lvItem ) );

	// Initialise list view item structure
	lvItem.mask			= LVIF_TEXT;
	lvItem.cchTextMax	= dwMaximumTextLength;
	lvItem.iItem		= nWhichItem;
	lvItem.iSubItem		= nWhichSubItem;
	lvItem.pszText		= ( LPTSTR )lpszItemText;

	// Add item to list view window
	bResult = SendMessage( g_hWndListView, LVM_SETITEM, ( WPARAM )NULL, ( LPARAM )&lvItem );

	return bResult;

} // End of function ListViewWindowSetItemText

void ListViewWindowSetFont( HFONT hFont )
{
	// Set list view window font
	SendMessage( g_hWndListView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListViewWindowSetFont
