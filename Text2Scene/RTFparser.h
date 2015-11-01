//////////////////////////////////////////////////////////////////////
// RTFparser.h: RTF parser class header
//////////////////////////////////////////////////////////////////////

#ifndef _IN_RTFREADER_
#define _IN_RTFREADER_

#include <iostream>

// RTF parser error codes
#define ecOK 0					// Everything's fine!
#define ecStackUnderflow 1		// Unmatched '}'
#define ecStackOverflow 2		// Too many '{' -- memory exhausted
#define ecUnmatchedBrace 3		// RTF ended during an open group.
#define ecInvalidHex 4			// invalid hex character found in data
#define ecBadTable 5			// RTF table (sym or prop) invalid
#define ecAssertion 6			// Assertion failure
#define ecEndOfFile 7			// End of file reached while reading RTF

// RTF struct & enum defines
typedef char fBool;

#define fTrue 1
#define fFalse 0

typedef struct char_prop
{
	char fBold;
	char fUnderline;
	char fItalic;
} CHP; // CHaracter Properties

typedef enum {justL, justR, justC, justF } JUST;

typedef struct para_prop
{
	int xaLeft;			// left indent in twips
	int xaRight;		// right indent in twips
	int xaFirst;		// first line indent in twips
	JUST just;			// justification
} PAP; // PAragraph Properties

typedef enum {sbkNon, sbkCol, sbkEvn, sbkOdd, sbkPg} SBK;

typedef enum {pgDec, pgURom, pgLRom, pgULtr, pgLLtr} PGN;

typedef struct sect_prop
{
	int cCols;			// number of columns
	SBK sbk;			// section break type
	int xaPgn;			// x position of page number in twips
	int yaPgn;			// y position of page number in twips
	PGN pgnFormat;		// how the page number is formatted
} SEP; // SEction Properties

typedef struct doc_prop
{
	int xaPage;			// page width in twips
	int yaPage;			// page height in twips
	int xaLeft;			// left margin in twips
	int yaTop;			// top margin in twips
	int xaRight;		// right margin in twips
	int yaBottom;		// bottom margin in twips
	int pgnStart;		// starting page number in twips
	char fFacingp;		// facing pages enabled?
	char fLandscape;	// landscape or portrait?
} DOP; // DOcument Properties

typedef enum { rdsNorm, rdsSkip } RDS; // Rtf Destination State

typedef enum { risNorm, risBin, risHex } RIS; // Rtf Internal State

typedef struct save // property save structure
{
	struct save *pNext; // next save
	CHP chp;
	PAP pap;
	SEP sep;
	DOP dop;
	RDS rds;
	RIS ris;
} SAVE;

// What types of properties are there?
typedef enum {
	ipropBold, ipropItalic, ipropUnderline, ipropLeftInd,
	ipropRightInd, ipropFirstInd, ipropCols, ipropPgnX,
	ipropPgnY, ipropXaPage, ipropYaPage, ipropXaLeft,
	ipropXaRight, ipropYaTop, ipropYaBottom, ipropPgnStart,
	ipropSbk, ipropPgnFormat, ipropFacingp, ipropLandscape,
	ipropJust, ipropPard, ipropPlain, ipropSectd,
	ipropMax 
} IPROP;

typedef enum {actnSpec, actnByte, actnWord} ACTN;

typedef enum {propChp, propPap, propSep, propDop} PROPTYPE;

typedef struct propmod
{
	ACTN actn;			// size of value
	PROPTYPE prop;		// structure containing value
	int offset;			// offset of value from base of structure
} PROP;

typedef enum {ipfnBin, ipfnHex, ipfnSkipDest } IPFN;

typedef enum {idestPict, idestSkip } IDEST;

typedef enum {kwdChar, kwdDest, kwdProp, kwdSpec} KWD;

typedef struct symbol
{
	char *szKeyword;	// RTF keyword
	int dflt;			// default value to use
	fBool fPassDflt;	// true to use default value from this table
	KWD kwd;			// base action to take
	int idx; 			// index into property table if kwd == kwdProp
						// index into destination table if kwd == kwdDest
						// character to print if kwd == kwdChar
} SYM;

typedef struct SRTFStyle
{
	char*			szStyleDef ;
	char*			szStyleName ;
	SRTFStyle*		pNext ;

} STY ;

typedef struct SRTFObject
{
	SRTFStyle*		szObjectStyle ;
	char*			szObjectInfo ;

	int				szCpStart1 ;
	int				szCpEnd1 ;
	int				szCpStart2 ;
	int				szCpEnd2 ;

	SRTFObject*		pNext ;

} OBJ ;

// RTF class definition
class CRTFParser  
{
public:
	CRTFParser();
	virtual ~CRTFParser();

	// RTF parser declarations
	int		ecRtfParse(FILE *fp);
	SRTFObject* GetObjects() { return m_pObjectList; };

protected:
	int		ecPushRtfState(void);
	int		ecPopRtfState(void);
	int		ecParseRtfKeyword(FILE *fp);
	int		ecParseChar(int c);
	int		ecTranslateKeyword(char *szKeyword, int param, fBool fParam);
	int		ecPrintChar(int ch);
	int		ecEndGroupAction(RDS rds);
	int		ecApplyPropChange(IPROP iprop, int val);
	int		ecChangeDest(IDEST idest);
	int		ecParseSpecialKeyword(IPFN ipfn);
	int		ecParseSpecialProperty(IPROP iprop, int val);
	int		ecParseHexByte(void);

	SRTFStyle*		ecCreateStyle() ;
	SRTFStyle*		ecFindStyle(char*) ;
	SRTFObject*		ecCreateObject() ;

	SRTFStyle*		m_pStyleList ;
	SRTFObject*		m_pObjectList ;

	inline void _DELETE_OBJECTLIST()
	{
		SRTFObject* ptr = NULL ;
		while(m_pObjectList)
		{
			ptr = m_pObjectList->pNext ;
//			if ( m_pObjectList->szObjectDef!=NULL ) free( m_pObjectList->szObjectDef ) ;
			if ( m_pObjectList->szObjectInfo!=NULL )
			{
				free( m_pObjectList->szObjectInfo ) ;
			}
			delete m_pObjectList ;
			m_pObjectList = ptr ;
		};
		m_NumObject = 0 ;
	};

	inline void _DELETE_STYLELIST()
	{
		SRTFStyle* ptr = NULL ;
		while(m_pStyleList)
		{
			ptr = m_pStyleList->pNext ;
			if ( m_pStyleList->szStyleDef != NULL )
			{
				free( m_pStyleList->szStyleDef ) ;
			}
			if ( m_pStyleList->szStyleName != NULL )
			{
				free( m_pStyleList->szStyleName ) ;
			}
			delete m_pStyleList ;
			m_pStyleList = ptr ;
		};
		m_NumStyle = 0 ;
	};

private:
	fBool			m_StyleState ;
	fBool			m_ParStateBegin ;	// flag double �tats signalant le d�but (\pard) et (\par) d'un paragraphe

	SRTFStyle*		m_pStyleCurrent ;
	SRTFObject*		m_pObjectCurrent ;

	unsigned int	m_NumObject ;
	unsigned int	m_NumStyle ;

	int		cGroup;
	fBool	fSkipDestIfUnk;
	long	cbBin;
	long	lParam;
	RDS		rds;
	RIS		ris;
	CHP		chp;
	PAP		pap;
	SEP		sep;
	DOP		dop;
	SAVE	*psave;

};

// end
#endif
