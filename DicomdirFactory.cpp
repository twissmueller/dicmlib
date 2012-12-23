#include "DicomdirFactory.hpp"

#include "DicomLibException.hpp"
#include <sstream>

DicomdirFactory::DicomdirFactory()
{
	ofname = "DICOMDIR";
	dicomdirProfile = EDDP_GeneralPurpose;
	fsid = DEFAULT_FSID;
	fsdfid = "";
	scsfsdf = DEFAULT_SCSFSDF;
	iconPrefix = "";
	defaultIcon = "";
	verbosemode = OFFalse;
	writeDicomdir = OFTrue;
	appendMode = OFFalse;
	createBackupFile = OFTrue;
	inventAttributes = OFFalse;
	mapFilenames = OFFalse;
	recurseFilesystem = OFFalse;
	resolutionCheck = OFTrue;
	addIconImage = OFFalse;
	inventPatientID = OFFalse;
	abortInconsistFile = OFFalse;
	E_EncodingType lengthEncoding = EET_ExplicitLength;
	E_GrpLenEncoding groupLengthEncoding = EGL_withoutGL;
}

const char* DicomdirFactory::getProfileName(E_DicomDirProfile profile)
{
    const char *result = "";
    switch(profile)
    {
        case EDDP_BasicCardiac:
            result = "STD-XABC-CD";
            break;
        case EDDP_XrayAngiographic:
            result = "STD-XA1K-CD";
            break;
        case EDDP_CTandMR:
            result = "STD-CTMR-xxxx";
            break;
        case EDDP_UltrasoundIDSF:
            result = "STD-US-ID-SF-xxxx";
            break;
        case EDDP_UltrasoundSCSF:
            result = "STD-US-SC-SF-xxxx";
            break;
        case EDDP_UltrasoundCCSF:
            result = "STD-US-CC-SF-xxxx";
            break;
        case EDDP_UltrasoundIDMF:
            result = "STD-US-ID-MF-xxxx";
            break;
        case EDDP_UltrasoundSCMF:
            result = "STD-US-SC-MF-xxxx";
            break;
        case EDDP_UltrasoundCCMF:
            result = "STD-US-CC-MF-xxxx";
            break;
        case EDDP_TwelveLeadECG:
            result = "STD-WVFM-ECG-FD";
            break;
        case EDDP_HemodynamicWaveform:
            result = "STD-WVFM-HD-FD";
            break;
        case EDDP_GeneralPurpose:
            result = "STD-GEN-CD/DVD-RAM";
            break;
        case EDDP_None:
            result = "NONE";
            break;
    }
    return result;
}

OFBool DicomdirFactory::isaValidCharSetName(const OFString& cs)
{
    const char* dcmCharSetNames[] = {
        "ISO_IR 100",
        "ISO_IR 101",
        "ISO_IR 109",
        "ISO_IR 110",
        "ISO_IR 144",
        "ISO_IR 127",
        "ISO_IR 126",
        "ISO_IR 138",
        "ISO_IR 148"
    };
    int numberOfDcmCharSetNames =
        (sizeof(dcmCharSetNames) / sizeof(dcmCharSetNames[0]));

    OFBool found = OFFalse;
    for (int i=0; i<numberOfDcmCharSetNames && !found; i++) {
        found = (strcmp(cs.c_str(), dcmCharSetNames[i]) == 0);
    }
    return found;
}

DcmDicomDir* DicomdirFactory::Create(std::string& _path, const std::vector<std::string>& _filenames)
{
	DcmDicomDir* ddir = NULL;
	path = _path;

#ifdef HAVE_GUSI_H
    GUSISetup(GUSIwithSIOUXSockets);
    GUSISetup(GUSIwithInternetSockets);
#endif

    int opt_debugMode = 0;
    SetDebugLevel(( 0 ));

//    //OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION, OFFIS_CONSOLE_DESCRIPTION, rcsid);
//    //OFCommandLine cmd;
//
//    //cmd.setOptionColumns(LONGCOL, SHORTCOL);
//    //cmd.setParamColumn(LONGCOL + SHORTCOL + 4);
//
//    cmd.addParam("dcmfile-in", "referenced DICOM file", OFCmdParam::PM_MultiMandatory);
//
//    cmd.addGroup("general options:", LONGCOL, SHORTCOL + 2);
//     cmd.addOption("--help",                    "-h",     "print this help text and exit");
//     cmd.addOption("--version",                           "print version information and exit", OFTrue /* exclusive */);
//     cmd.addOption("--verbose",                 "-v",     "verbose mode, print processing details");
//     cmd.addOption("--debug",                   "-d",     "debug mode, print debug information");
//
//    cmd.addGroup("input options:");
//      cmd.addSubGroup("DICOMDIR identifiers:");
//        cmd.addOption("--output-file",          "+D",  1, "[f]ilename: string",
//                                                          "generate specific DICOMDIR file\n(default: DICOMDIR in current directory)");
//        cmd.addOption("--fileset-id",           "+F",  1, "[i]d: string (default: " DEFAULT_FSID ")",
//                                                          "use specific file set ID");
//        cmd.addOption("--descriptor",           "+R",  1, "[f]ilename: string",
//                                                          "add a file set descriptor file ID\n(e.g. " DEFAULT_FSDFID ", default: no descriptor)");
//        cmd.addOption("--char-set",             "+C",  1, "[c]har-set: string",
//                                                          "add a specific character set for descriptor\n(default: \"" DEFAULT_SCSFSDF "\" if descriptor present)");
//      cmd.addSubGroup("type 1 attributes:");
//        cmd.addOption("--strict",               "-I",     "exit with error if DICOMDIR type 1 attributes\nare missing in DICOM file (default)");
//        cmd.addOption("--invent",               "+I",     "invent DICOMDIR type 1 attributes\nif missing in DICOM file");
//      cmd.addSubGroup("reading:");
//        cmd.addOption("--keep-filenames",       "-m",     "expect filenames to be in DICOM format\n(default)");
//        cmd.addOption("--map-filenames",        "+m",     "map to DICOM filenames (lowercase->uppercase,\nand remove trailing period)");
//        cmd.addOption("--no-recurse",           "-r",     "do not recurse within directories (default)");
//        cmd.addOption("--recurse",              "+r",     "recurse within filesystem directories");
//      cmd.addSubGroup("checking:");
//        cmd.addOption("--warn-inconsist-files", "+W",     "warn about inconsistant files (default)");
//        cmd.addOption("--abort-inconsist-file", "-a",     "abort on first inconsistant file");
//        cmd.addOption("--invent-patient-id",    "+Ipi",   "invent new PatientID in case of inconsistant\nPatientsName attributes");
//#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
//        cmd.addOption("--no-resolution-check",  "-Nrc",   "do not reject images with non-standard\nspatial resolution (just warn)");
//      cmd.addSubGroup("icon images:");
//        cmd.addOption("--add-icon-image",       "+X",     "add monochrome icon image on IMAGE level\n(default for cardiac profiles)");
//        cmd.addOption("--icon-file-prefix",     "-Xi", 1, "[p]refix: string",
//                                                          "use PGM image 'prefix'+'dcmfile-in' as icon\n(default: create icon from DICOM image)");
//        cmd.addOption("--default-icon",         "-Xd", 1, "[f]ilename: string",
//                                                          "use specified PGM image if icon cannot be\ncreated automatically (default: black image)");
//#endif
//    cmd.addGroup("output options:");
//#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
//      cmd.addSubGroup("profiles:");
//        cmd.addOption("--general-purpose",      "-Pgp",   "General Purpose Interchange on CD-R or\nDVD-RAM Media (STD-GEN-CD/DVD-RAM, default)");
//        cmd.addOption("--no-profile",           "-Pno",   "like --general-purpose without restricting\nthe transfer syntax (also compressed files)");
//        cmd.addOption("--basic-cardiac",        "-Pbc",   "Basic Cardiac X-Ray Angiographic Studies on\nCD-R Media (STD-XABC-CD)");
//        cmd.addOption("--xray-angiographic",    "-Pxa",   "1024 X-Ray Angiographic Studies on CD-R Media\n(STD-XA1K-CD)");
//        cmd.addOption("--ct-and-mr",            "-Pcm",   "CT/MR Studies (STD-CTMR-xxxx)");
//        cmd.addOption("--ultrasound-id-sf",     "-Pus",   "Ultrasound Single Frame for Image Display\n(STD-US-ID-SF-xxxx)");
//        cmd.addOption("--ultrasound-sc-sf",               "Ultrasound Single Frame with Spatial\nCalibration (STD-US-SC-SF-xxxx)");
//        cmd.addOption("--ultrasound-cc-sf",               "Ultrasound Single Frame with Combined\nCalibration (STD-US-CC-SF-xxxx)");
//        cmd.addOption("--ultrasound-id-mf",     "-Pum",   "Ultrasound Single & Multi-Frame for Image\nDisplay (STD-US-ID-MF-xxxx)");
//        cmd.addOption("--ultrasound-sc-mf",               "Ultrasound Single & Multi-Frame with Spatial\nCalibration (STD-UD-SC-MF-xxxx)");
//        cmd.addOption("--ultrasound-cc-mf",               "Ultrasound Single & Multi-Frame with Combined\nCalibration (STD-UD-CC-MF-xxxx)");
//        cmd.addOption("--12-lead-ecg",          "-Pec",   "12-lead ECG Interchange on Diskette\n(STD-WVFM-ECG-FD)");
//        cmd.addOption("--hemodynamic-waveform", "-Phd",   "Hemodynamic Waveform Interchange on Diskette\n(STD-WVFM-HD-FD)");
//#endif
//      cmd.addSubGroup("writing:");
//        cmd.addOption("--replace",              "-A",     "replace existing DICOMDIR (default)");
//        cmd.addOption("--append",               "+A",     "append to existing DICOMDIR");
//        cmd.addOption("--discard",              "-w",     "do not write out DICOMDIR");
//        cmd.addOption("--no-backup",            "-nb",    "do not create a backup of existing DICOMDIR");
//      cmd.addSubGroup("post-1993 value representations:");
//        cmd.addOption("--enable-new-vr",        "+u",     "enable support for new VRs (UN/UT) (default)");
//        cmd.addOption("--disable-new-vr",       "-u",     "disable support for new VRs, convert to OB");
//      cmd.addSubGroup("group length encoding:");
//        cmd.addOption("--group-length-remove",  "-g",     "write without group length elements (default)");
//        cmd.addOption("--group-length-create",  "+g",     "write with group length elements");
//      cmd.addSubGroup("length encoding in sequences and items:");
//        cmd.addOption("--length-explicit",      "+e",     "write with explicit lengths (default)");
//        cmd.addOption("--length-undefined",     "-e",     "write with undefined lengths");

//    /* evaluate command line */
//    prepareCmdLineArgs(argc, argv, OFFIS_CONSOLE_APPLICATION);
//    if (app.parseCommandLine(cmd, argc, argv, OFCommandLine::ExpandWildcards))
//    {
//      /* check exclusive options first */
//
//      if (cmd.getParamCount() == 0)
//      {
//          if (cmd.findOption("--version"))
//          {
//			  // noch nicht betrachtet
//              app.printHeader(OFTrue /*print host identifier*/);          // uses ofConsole.lockCerr()
//              CERR << endl << "External libraries used:";
//#if !defined(WITH_ZLIB) && !defined(BUILD_DCMGPDIR_AS_DCMMKDIR)
//              CERR << " none" << endl;
//#else
//              CERR << endl;
//#endif
//#ifdef WITH_ZLIB
//              CERR << "- ZLIB, Version " << zlibVersion() << endl;
//#endif
//#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
//              CERR << "- " << DiJPEGPlugin::getLibraryVersionString() << endl;
//#endif
//              return 0;
//          }
//      }
//
//      /* general options */
//
//      if (cmd.findOption("--verbose")) verbosemode = OFTrue;
//      if (cmd.findOption("--debug")) opt_debugMode = 5;
//
//      if (cmd.findOption("--output-file")) app.checkValue(cmd.getValue(ofname));
//      if (cmd.findOption("--fileset-id")) app.checkValue(cmd.getValue(fsid));
//      if (cmd.findOption("--descriptor")) app.checkValue(cmd.getValue(fsdfid));
//      if (cmd.findOption("--char-set"))
//      {
//          app.checkValue(cmd.getValue(scsfsdf));
//          if (!isaValidCharSetName(scsfsdf))
//          {
//            OFString aString = "unknown char-set: ";
//            aString += scsfsdf;
//            app.printError(aString.c_str());
//          }
//      }
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--strict")) inventAttributes = OFFalse;
//      if (cmd.findOption("--invent")) inventAttributes = OFTrue;
//      cmd.endOptionBlock();
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--keep-filenames")) mapFilenames = OFFalse;
//      if (cmd.findOption("--map-filenames")) mapFilenames = OFTrue;
//      cmd.endOptionBlock();
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--no-recurse")) recurseFilesystem = OFFalse;
//      if (cmd.findOption("--recurse")) recurseFilesystem = OFTrue;
//      cmd.endOptionBlock();
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--warn-inconsist-files")) abortInconsistFile = OFFalse;
//      if (cmd.findOption("--abort-inconsist-file")) abortInconsistFile = OFTrue;
//      cmd.endOptionBlock();
//      if (cmd.findOption("--invent-patient-id")) inventPatientID = OFTrue;
//#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
//      if (cmd.findOption("--no-resolution-check")) resolutionCheck = OFFalse;
//
//      if (cmd.findOption("--add-icon-image")) addIconImage = OFTrue;
//      if (cmd.findOption("--icon-file-prefix")) app.checkValue(cmd.getValue(iconPrefix));
//      if (cmd.findOption("--default-icon")) app.checkValue(cmd.getValue(defaultIcon));
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--general-purpose")) dicomdirProfile = EDDP_GeneralPurpose;
//      if (cmd.findOption("--no-profile")) dicomdirProfile = EDDP_None;
//      if (cmd.findOption("--basic-cardiac"))
//      {
//          dicomdirProfile = EDDP_BasicCardiac;
//          addIconImage = OFTrue;
//      }
//      if (cmd.findOption("--xray-angiographic"))
//      {
//          dicomdirProfile = EDDP_XrayAngiographic;
//          addIconImage = OFTrue;
//      }
//      if (cmd.findOption("--ct-and-mr")) dicomdirProfile = EDDP_CTandMR;
//      if (cmd.findOption("--ultrasound-id-sf")) dicomdirProfile = EDDP_UltrasoundIDSF;
//      if (cmd.findOption("--ultrasound-sc-sf")) dicomdirProfile = EDDP_UltrasoundSCSF;
//      if (cmd.findOption("--ultrasound-cc-sf")) dicomdirProfile = EDDP_UltrasoundCCSF;
//      if (cmd.findOption("--ultrasound-id-mf")) dicomdirProfile = EDDP_UltrasoundIDMF;
//      if (cmd.findOption("--ultrasound-sc-mf")) dicomdirProfile = EDDP_UltrasoundSCMF;
//      if (cmd.findOption("--ultrasound-cc-mf")) dicomdirProfile = EDDP_UltrasoundCCMF;
//      if (cmd.findOption("--12-lead-ecg")) dicomdirProfile = EDDP_TwelveLeadECG;
//      if (cmd.findOption("--hemodynamic-waveform")) dicomdirProfile = EDDP_HemodynamicWaveform;
//      cmd.endOptionBlock();
//#endif
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--replace"))
//      {
//        writeDicomdir = OFTrue;
//        appendMode = OFFalse;
//      }
//      if (cmd.findOption("--append"))
//      {
//        writeDicomdir = OFTrue;
//        appendMode = OFTrue;
//      }
//      if (cmd.findOption("--discard"))
//      {
//        writeDicomdir = OFFalse;
//        appendMode = OFFalse;
//      }
//      cmd.endOptionBlock();
//      if (cmd.findOption("--no-backup"))
//        createBackupFile = OFFalse;
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--enable-new-vr"))
//      {
//        dcmEnableUnknownVRGeneration.set(OFTrue);
//        dcmEnableUnlimitedTextVRGeneration.set(OFTrue);
//      }
//      if (cmd.findOption("--disable-new-vr"))
//      {
//        dcmEnableUnknownVRGeneration.set(OFFalse);
//        dcmEnableUnlimitedTextVRGeneration.set(OFFalse);
//      }
//      cmd.endOptionBlock();
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--group-length-create")) groupLengthEncoding = EGL_withGL;
//      if (cmd.findOption("--group-length-remove")) groupLengthEncoding = EGL_withoutGL;
//      cmd.endOptionBlock();
//
//      cmd.beginOptionBlock();
//      if (cmd.findOption("--length-explicit")) lengthEncoding = EET_ExplicitLength;
//      if (cmd.findOption("--length-undefined")) lengthEncoding = EET_UndefinedLength;
//      cmd.endOptionBlock();
//
//    }

    const char *current = NULL;
    OFList<OFString> fnames;
    //int count = cmd.getParamCount();
	int filesnum = _filenames.size();
    for (int i=0; i<filesnum; ++i)
    {
      //cmd.getParam(i, current);
		fnames.push_back(_filenames[i].c_str());
    }

    SetDebugLevel((opt_debugMode));

    /* make sure data dictionary is loaded */
    if (!dcmDataDict.isDictionaryLoaded()) {
		CERR << "Error: no data dictionary loaded, "
             << "check environment variable: "
             << DCM_DICT_ENVIRONMENT_VARIABLE << endl;
        return NULL; /* DcmDicomDir class dumps core when no data dictionary */
    }

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
    // register global decompression codecs
    DJDecoderRegistration::registerCodecs();
#endif

    OFBool ok = OFTrue;
    if (recurseFilesystem) {
        OFList<OFString> expandedNames;
        ok = expandFileNames(fnames, expandedNames);
        if (ok) {
            ddir = createDicomdirFromFiles(expandedNames);
        }
    } else {
        ddir = createDicomdirFromFiles(fnames);
    }

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
    // deregister global decompression codecs
    DJDecoderRegistration::cleanup();
#endif

#ifdef DEBUG
    dcmDataDict.clear();  /* useful for debugging with dmalloc */
#endif
    return ddir;
}

OFBool DicomdirFactory::dcmTagExists(DcmItem* d, const DcmTagKey& key)
{
    return d->tagExists(key, OFFalse);
}

OFBool DicomdirFactory::dcmTagExistsWithValue(DcmItem* d, const DcmTagKey& key)
{
    return d->tagExistsWithValue(key, OFFalse);
}

OFString DicomdirFactory::dcmFindString(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub)
{
    OFString s;

    OFCondition ec = d->findAndGetOFStringArray(key, s, searchIntoSub);
    if (ec != EC_Normal && ec != EC_TagNotFound) {
        DcmTag tag(key);
        CERR << "dcmFindString: error while finding " << tag.getTagName()
             << " " << key << ": "
             << ec.text() << endl;
    }

    return s;
}

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
long DicomdirFactory::dcmFindInteger(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub)
{
    long i = 0;

    DcmStack stack;
    E_Condition ec = d->search(key, stack, ESM_fromHere, searchIntoSub);
    DcmElement *elem = (DcmElement*) stack.top();
    if (ec == EC_Normal && elem != NULL) {
        switch (elem->ident()) {
        case EVR_UL:
        case EVR_up:
            Uint32 ul;
            ec = elem->getUint32(ul, 0 /*which*/);
            i = ul;
            break;
        case EVR_SL:
        case EVR_IS:
            Sint32 sl;
            ec = elem->getSint32(sl, 0 /*which*/);
            i = sl;
            break;
        case EVR_US:
        case EVR_xs:
            Uint16 us;
            ec = elem->getUint16(us, 0 /*which*/);
            i = us;
            break;
        case EVR_SS:
            Sint16 ss;
            ec = elem->getSint16(ss, 0 /*which*/);
            i = ss;
            break;
        default:
            ec = EC_IllegalCall;
            break;
        }
    }

    if (ec != EC_Normal && ec != EC_TagNotFound) {
        DcmTag tag(key);
        CERR << "dcmFindInteger: error while finding " << tag.getTagName()
             << " " << key << ": "
             << ec.text() << endl;
    }

    return i;
}
#endif
OFString DicomdirFactory::dcmFindStringInFile(const OFString& fname, const DcmTagKey& key, OFBool searchIntoSub)
{
    if (verbosemode) {
        COUT << "investigating file: " << fname << endl;
    }

    DcmFileFormat ff;
    OFCondition cond = ff.loadFile(fname.c_str());

    if (cond.bad()) {
        CERR << "Error: "
             << cond.text()
             << ": reading file: " << fname << endl;
		return OFFalse;
	}

    OFString s = dcmFindString(ff.getDataset(), key, searchIntoSub);

    return s;
}

OFBool DicomdirFactory::dcmInsertString(DcmItem* d, const DcmTagKey& key, const OFString& s, OFBool replaceOld)
{
    if (d == NULL) {
        CERR << "Error: dcmInsertString: null DcmItem argument" << endl;
        return OFFalse;
    }

    DcmTag tag(key);
    DcmElement *elem = newDicomElement(tag);
    OFCondition cond = EC_Normal;

    if (elem == NULL) {
        CERR << "Error: dcmInsertString: cannot create DcmElement" << endl;
        return OFFalse;
    }
    if (!s.empty()) {
        cond = elem->putOFStringArray(s);
        if (cond != EC_Normal) {
            CERR << "Error: dcmInsertString: cannot put string" << endl;
            return OFFalse;
        }
    }
    cond = d->insert(elem, replaceOld);
    if (cond != EC_Normal) {
        CERR << "Error: dcmInsertString: cannot insert element" << endl;
        delete elem;
        return OFFalse;
    }

    return (cond == EC_Normal);
}
OFBool DicomdirFactory::dcmInsertInteger(DcmItem* d, const DcmTagKey& key, const long i, OFBool replaceOld)
{
    if (d == NULL) {
        CERR << "Error: dcmInsertInteger: null DcmItem argument" << endl;
        return OFFalse;
    }

    DcmTag tag(key);
    DcmElement *elem = newDicomElement(tag);
    E_Condition cond = EC_Normal;

    if (elem == NULL) {
        CERR << "Error: dcmInsertInteger: cannot create DcmElement" << endl;
        return OFFalse;
    }
    switch(tag.getEVR())
    {
        case EVR_US:
            cond = elem->putUint16((Uint16)i);
            break;
        /* might be extended later: EVR_SS, EVR_IS, ... */
        default:
            break;
    }
    if (cond != EC_Normal) {
        CERR << "Error: dcmInsertInteger: cannot put integer" << endl;
        return OFFalse;
    }
    cond = d->insert(elem, replaceOld);
    if (cond != EC_Normal) {
        CERR << "Error: dcmInsertInteger: cannot insert element" << endl;
        return OFFalse;
    }

    return (cond == EC_Normal);
}
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
OFBool DicomdirFactory::dcmCopyInteger(DcmItem* sink, const DcmTagKey& key, DcmItem* source)
{
    return dcmInsertInteger(sink, key, dcmFindInteger(source, key));
}
#endif
OFBool DicomdirFactory::dcmCopyString(DcmItem* sink, const DcmTagKey& key, DcmItem* source)
{
    return dcmInsertString(sink, key, dcmFindString(source, key));
}

OFBool DicomdirFactory::dcmCopyOptString(DcmItem* sink, const DcmTagKey& key, DcmItem* source)
{
    OFBool ok = OFTrue;
    if (dcmTagExists(source, key)) {
        ok = dcmInsertString(sink, key, dcmFindString(source, key));
    }
    return ok;
}

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
OFBool DicomdirFactory::dcmCopyStringWithDefault(DcmItem* sink, const DcmTagKey& key, DcmItem* source, const char* defaultString)
{
    OFBool ok = OFTrue;
    if (dcmTagExistsWithValue(source, key)) {
        ok = dcmInsertString(sink, key, dcmFindString(source, key));
    } else {
        ok = dcmInsertString(sink, key, defaultString);
    }
    return ok;
}
#endif
OFBool DicomdirFactory::dcmCopySequence(DcmItem* sink, const DcmTagKey& key, DcmItem* source)
{
    OFBool ok = OFTrue;
    DcmTag tag(key);

    if (tag.getEVR() != EVR_SQ) {
        CERR << "internal Error: dcmCopySequence: "
             << key << " not SQ" << endl;
        abort();
    }

    if (dcmTagExists(source, key)) {
        DcmSequenceOfItems *sq = NULL;
        DcmStack stack;
        OFCondition ec = EC_Normal;

        ec = source->search(key, stack, ESM_fromHere, OFFalse);
        sq = (DcmSequenceOfItems*) stack.top();
        if (ec == EC_Normal && sq != NULL) {
            // make a copy of sq
            DcmSequenceOfItems *sqcopy = new DcmSequenceOfItems(*sq);
            // insert sqcopy into the sink
            ec = sink->insert(sqcopy, OFTrue);
            if (ec != EC_Normal) {
                CERR << "Error: dcmCopySequence: cannot insert element" << endl;
                delete sqcopy;
                ok = OFFalse;
            }
        } else {
            ok = OFFalse;
        }
    } else {
        CERR << "dcmCopySequence: error while finding " << tag.getTagName() << " " << key << endl;
        ok = OFFalse;
    }
    return ok;
}

OFBool DicomdirFactory::dcmCopyOptSequence(DcmItem* sink, const DcmTagKey& key, DcmItem* source)
{
    if (dcmTagExists(source, key)) return dcmCopySequence(sink, key, source);
    return OFTrue;
}

OFString DicomdirFactory::currentDate()
{
    OFString dateString;
    if (!DcmDate::getCurrentDate(dateString).good())
    {
        CERR << "ERROR: cannot get current date" << endl;
        return "";
    }
    return dateString;
}

OFString DicomdirFactory::currentTime()
{
    OFString timeString;
    if (!DcmTime::getCurrentTime(timeString).good())
    {
        CERR << "ERROR: cannot get current time" << endl;
        return "";
    }
    return timeString;
}

OFString DicomdirFactory::alternativeStudyDate(DcmItem* d)
{
    /* use another date if present */
    OFString date = dcmFindString(d, DCM_SeriesDate);
    if (!date.empty()) return date;
    date = dcmFindString(d, DCM_AcquisitionDate);
    if (!date.empty()) return date;
    date = dcmFindString(d, DCM_ContentDate);
    if (!date.empty()) return date;
    /* use current date */
    return currentDate();
}

OFString DicomdirFactory::alternativeStudyTime(DcmItem* d)
{
    /* use another time if present */
    OFString timestr = dcmFindString(d, DCM_SeriesTime);
    if (!timestr.empty()) return timestr;
    timestr = dcmFindString(d, DCM_AcquisitionTime);
    if (!timestr.empty()) return timestr;
    timestr = dcmFindString(d, DCM_ContentTime);
    if (!timestr.empty()) return timestr;
    /* use current time */
    return currentTime();
}

OFString DicomdirFactory::defaultNumber(int number)
{
    char numbuf[64];
    unsigned long ul = (unsigned long)number;
    sprintf(numbuf, "%lu", ul);
    return numbuf;
}

OFString DicomdirFactory::defaultID(const OFString& prefix, int number)
{
    /* use a most 10 chars from prefix */
    OFString idbuf(prefix, 0, 10);
    unsigned long ul = (unsigned long)number;
    char nbuf[16];
    sprintf(nbuf, "%06lu", ul);
    idbuf.append(nbuf);
    return idbuf;
}


void DicomdirFactory::hostToDicomFilename(OFString& fname)
{
    /*
    ** Massage filename into dicom format
    ** (dos conventions for path separators)
    */
    OFString newname;
    int len = fname.size();
    for (int i=0; i<len; i++) {
        char c = fname[i];
        /* the PATH_SEPARATOR depends on the OS (see <osconfig.h>) */
        if (c == PATH_SEPARATOR) {
            newname += '\\';
        } else if (isalpha(c) || isdigit(c) || (c == '_') || (c == '\\')) {
            /* filenames in DICOM must always be in uppercase */
            newname += toupper(c);
        }
    }
    fname = newname;
}

void DicomdirFactory::dicomToHostFilename(OFString& fname, OFBool mapToLower)
{
    /*
    ** Massage filename into machine format
    ** (replace dos conventions for path separators)
    */
    int len = fname.size();
    for (int i=0; i<len; i++) {
        char c = fname[i];
        /* the PATH_SEPARATOR depends on the OS (see <osconfig.h>) */
        if (c == '\\') {
            c = PATH_SEPARATOR;
        } else {
            if (mapToLower) {
                c = tolower(c);
            }
        }
        fname[i] = c;
    }
}

int DicomdirFactory::componentCount(const OFString& fname, char separator)
{
    if (fname.empty()) return 0;
    int n = 1;
    int len = fname.size();
    for (int i=0; i<len; i++) {
        if (fname[i] == separator) {
            n++;
        }
    }
    return n;
}

OFBool DicomdirFactory::isComponentTooLarge(const OFString& fname, int componentLimit, char separator)
{
    if (fname.empty()) return -1;
    int n = 0;
    const char*  s = fname.c_str();
    const char* ss = (const char*)strchr(s, separator);
    while (ss) {
        int diff = (ss - s);
        if (diff > componentLimit) {
            return OFTrue;
        }
        s = ss+1;
        ss = (const char*)strchr(s, separator);
        n++;
    }
    int len = strlen(s);
    if (mapFilenames && s[len-1] == '.') {
        /* disregard trailing point */
        len--;
    }
    if (len > componentLimit) {
        return OFTrue;
    }

    return OFFalse;
}

OFBool DicomdirFactory::cmp(const OFString& s1, const OFString& s2)
{
    if (s1.empty() || s2.empty()) {
        return OFFalse;
    }
    return (s1.compare(s2) == 0)?(OFTrue):(OFFalse);
}

OFBool DicomdirFactory::checkExists(DcmItem* d, const DcmTagKey& key, const OFString& fname)
{
    if (!dcmTagExists(d, key)) {
        DcmTag tag(key);
        CERR << "Error: required attribute " << tag.getTagName()
             << " " << key << " missing in file: "
             << fname << endl;
        return OFFalse;
    }
    return OFTrue;
}

OFBool DicomdirFactory::checkExistsWithValue(DcmItem* d, const DcmTagKey& key, const OFString& fname)
{
    if (!checkExists(d, key, fname)) {
        return OFFalse;
    }
    OFString s = dcmFindString(d, key);
    if (s.empty()) {
        DcmTag tag(key);
        CERR << "Error: required attribute " << tag.getTagName()
             << " " << key << " has no value in file: "
             << fname << endl;
        return OFFalse;
    }
    return OFTrue;
}

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
OFBool DicomdirFactory::checkExistsWithStringValue(DcmItem* d, const DcmTagKey& key, const OFString &value, const OFString& fname, OFBool normalize)
{
    if (!checkExists(d, key, fname)) {
        return OFFalse;
    }
    OFString s = dcmFindString(d, key);
    if (normalize)
        normalizeString(s, OFTrue /* multiPart */, OFTrue /*leading */, OFTrue /* trailing */);
    if (!cmp(s, value)) {
        DcmTag tag(key);
        CERR << "Error: attribute " << tag.getTagName()
             << " " << key << " has other value than expected in file: "
             << fname << endl;
        return OFFalse;
    }
    return OFTrue;
}
OFBool DicomdirFactory::checkExistsWithIntegerValue(DcmItem* d, const DcmTagKey& key, const long value, const OFString& fname)
{
    if (!checkExists(d, key, fname)) {
        return OFFalse;
    }
    long i = dcmFindInteger(d, key);
    if (i != value) {
        DcmTag tag(key);
        CERR << "Error: attribute " << tag.getTagName()
             << " " << key << " has other value than expected in file: "
             << fname << endl;
        return OFFalse;
    }
    return OFTrue;
}

OFBool DicomdirFactory::checkExistsWithMinMaxValue(DcmItem* d, const DcmTagKey& key, const long min, const long max, const OFString& fname, const OFBool reject)
{
    if (!checkExists(d, key, fname)) {
        return OFFalse;
    }
    long i = dcmFindInteger(d, key);
    if ((i < min) || (i > max)) {
        DcmTag tag(key);
        if (reject)
        {
            CERR << "Error: attribute " << tag.getTagName()
                 << " " << key << " has other value than expected in file: "
                 << fname << endl;
            return OFFalse;
        } else {
            CERR << "warning: attribute " << tag.getTagName()
                 << " " << key << " has other value than expected in file: "
                 << fname << endl;
        }
    }
    return OFTrue;
}
#endif
OFString DicomdirFactory::recordTypeToName(E_DirRecType t)
{
    const char* s = NULL;
    switch (t) {
    case ERT_root:
        s = "Root"; break;
    case ERT_Curve:
        s = "Curve"; break;
    case ERT_FilmBox:
        s = "FilmBox"; break;
    case ERT_FilmSession:
        s = "FilmSession"; break;
    case ERT_Image:
        s = "Image"; break;
    case ERT_ImageBox:
        s = "ImageBox"; break;
    case ERT_Interpretation:
        s = "Interpretation"; break;
    case ERT_ModalityLut:
        s = "ModalityLUT"; break;
    case ERT_Mrdr:
        s = "MRDR"; break;
    case ERT_Overlay:
        s = "Overlay"; break;
    case ERT_Patient:
        s = "Patient"; break;
    case ERT_PrintQueue:
        s = "PrintQueue"; break;
    case ERT_Private:
        s = "Private"; break;
    case ERT_Results:
        s = "Results"; break;
    case ERT_Series:
        s = "Series"; break;
    case ERT_Study:
        s = "Study"; break;
    case ERT_StudyComponent:
        s = "StudyComponent"; break;
    case ERT_Topic:
        s = "Topic"; break;
    case ERT_Visit:
        s = "Visit"; break;
    case ERT_VoiLut:
        s = "VOILUT"; break;
    case ERT_StructReport:
        s = "StructReport"; break;
    case ERT_Presentation:
        s = "Presentation"; break;
    case ERT_Waveform:
        s = "Waveform"; break;
    case ERT_RTDose:
        s = "RTDose"; break;
    case ERT_RTStructureSet:
        s = "RTStructureSet"; break;
    case ERT_RTPlan:
        s = "RTPlan"; break;
    case ERT_RTTreatRecord:
        s = "RTTreatRecord"; break;
    case ERT_StoredPrint:
        s = "StoredPrint"; break;
    case ERT_KeyObjectDoc:
        s = "KeyObjectDoc"; break;
    default:
        s = "(unknown-directory-record-type)";
        break;
    }
    return s;
}

OFBool DicomdirFactory::checkImage(const OFString& fname, DcmFileFormat *ff)
{
	/*
    ** Do some sanity checks on the file.
    ** - is this a part 10 file format file?
    ** - can this file be part of a DICOMDIR according to the selected profile?
    ** - does it have the necessary attributes?
    ** - is the transfer syntax valid?
    **
    */

    if (ff == NULL) return OFFalse;

    OFBool ok = OFTrue;

	DcmMetaInfo *m = ff->getMetaInfo();
	// already checked in DicomImporter
    //if (m == NULL || m->card() == 0) {
    //    CERR << "Error: file not part 10 format (no meta-header): "
    //         << fname << endl;
    //    ok = OFFalse;
    //}

	DcmDataset *d = ff->getDataset();
    // already checked in DicomImporter
    //if (d == NULL) {
    //    CERR << "Error: file contains no data (no dataset): "
    //         << fname << endl;
    //    /* give up checking */
    //    return OFFalse;
    //}

    /*
    ** Is sop classs ok?
    */
    OFString mediaSOPClassUID = dcmFindString(m, DCM_MediaStorageSOPClassUID);
    if (mediaSOPClassUID.empty()) {
        CERR << "Error: MediaStorageSOPClassUID missing in meta-header: "
             << fname << endl;
        ok = OFFalse;
    }

    /*
    ** Check if the SOP Class is a known storage SOP class (an image, overlay,
    ** curve, etc.
    */
    OFBool found = OFFalse;
    OFString expectedTransferSyntax = UID_LittleEndianExplicitTransferSyntax;

    switch (dicomdirProfile)
    {
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
        case EDDP_BasicCardiac:
            if (cmp(mediaSOPClassUID, UID_XRayAngiographicImageStorage))
            {
                expectedTransferSyntax = UID_JPEGProcess14SV1TransferSyntax;
                found = OFTrue;
            } else {
                found = cmp(mediaSOPClassUID, UID_DetachedPatientManagementSOPClass);
            }
            break;
        case EDDP_XrayAngiographic:
            if (cmp(mediaSOPClassUID, UID_XRayAngiographicImageStorage))
            {
                expectedTransferSyntax = UID_JPEGProcess14SV1TransferSyntax;
                found = OFTrue;
            } else {
                found = cmp(mediaSOPClassUID, UID_SecondaryCaptureImageStorage) ||
                        cmp(mediaSOPClassUID, UID_StandaloneOverlayStorage) ||
                        cmp(mediaSOPClassUID, UID_StandaloneCurveStorage) ||
                        cmp(mediaSOPClassUID, UID_DetachedPatientManagementSOPClass);
            }
            break;
        case EDDP_CTandMR:
            /* transfer syntax needs to be checked later */
            found = cmp(mediaSOPClassUID, UID_CTImageStorage) ||
                    cmp(mediaSOPClassUID, UID_MRImageStorage) ||
                    cmp(mediaSOPClassUID, UID_SecondaryCaptureImageStorage) ||
                    cmp(mediaSOPClassUID, UID_DetachedPatientManagementSOPClass);
            break;
        case EDDP_UltrasoundIDSF:
        case EDDP_UltrasoundSCSF:
        case EDDP_UltrasoundCCSF:
            /* transfer syntax needs to be checked later */
            found = cmp(mediaSOPClassUID, UID_UltrasoundImageStorage);
            break;
        case EDDP_UltrasoundIDMF:
        case EDDP_UltrasoundSCMF:
        case EDDP_UltrasoundCCMF:
            /* transfer syntax needs to be checked later */
            found = cmp(mediaSOPClassUID, UID_UltrasoundImageStorage) ||
                    cmp(mediaSOPClassUID, UID_UltrasoundMultiframeImageStorage);
            break;
        case EDDP_TwelveLeadECG:
            found = cmp(mediaSOPClassUID, UID_TwelveLeadECGWaveformStorage);
        case EDDP_HemodynamicWaveform:
            found = cmp(mediaSOPClassUID, UID_HemodynamicWaveformStorage);
            break;
#endif
        case EDDP_GeneralPurpose:
        case EDDP_None:
        default:
        {
            /* is it an image ? */
            for (int i=0; i<numberOfDcmImageSOPClassUIDs && !found; i++) {
                found = cmp(mediaSOPClassUID, dcmImageSOPClassUIDs[i]);
            }
            /* is it an overlay/curve/modality_lut/voi_lut etc. ? */
            found = found || cmp(mediaSOPClassUID, UID_StandaloneOverlayStorage);
            found = found || cmp(mediaSOPClassUID, UID_StandaloneCurveStorage);
            found = found || cmp(mediaSOPClassUID, UID_StandaloneModalityLUTStorage);
            found = found || cmp(mediaSOPClassUID, UID_StandaloneVOILUTStorage);
            found = found || cmp(mediaSOPClassUID, UID_PETCurveStorage);
            found = found || cmp(mediaSOPClassUID, UID_GrayscaleSoftcopyPresentationStateStorage);
            found = found || cmp(mediaSOPClassUID, UID_StoredPrintStorage);
            /* is it one of the RT SOP Classes? */
            found = found || cmp(mediaSOPClassUID, UID_RTDoseStorage);
            found = found || cmp(mediaSOPClassUID, UID_RTStructureSetStorage);
            found = found || cmp(mediaSOPClassUID, UID_RTBeamsTreatmentRecordStorage);
            found = found || cmp(mediaSOPClassUID, UID_RTPlanStorage);
            found = found || cmp(mediaSOPClassUID, UID_RTBrachyTreatmentRecordStorage);
            found = found || cmp(mediaSOPClassUID, UID_RTTreatmentSummaryRecordStorage);
            /* is it one of the structured reporting SOP Classes? */
            found = found || cmp(mediaSOPClassUID, UID_BasicTextSR);
            found = found || cmp(mediaSOPClassUID, UID_EnhancedSR);
            found = found || cmp(mediaSOPClassUID, UID_ComprehensiveSR);
            found = found || cmp(mediaSOPClassUID, UID_MammographyCADSR);
            found = found || cmp(mediaSOPClassUID, UID_KeyObjectSelectionDocument);
            /* is it one of the waveform SOP Classes? */
            found = found || cmp(mediaSOPClassUID, UID_TwelveLeadECGWaveformStorage);
            found = found || cmp(mediaSOPClassUID, UID_GeneralECGWaveformStorage);
            found = found || cmp(mediaSOPClassUID, UID_AmbulatoryECGWaveformStorage);
            found = found || cmp(mediaSOPClassUID, UID_HemodynamicWaveformStorage);
            found = found || cmp(mediaSOPClassUID, UID_CardiacElectrophysiologyWaveformStorage);
            found = found || cmp(mediaSOPClassUID, UID_BasicVoiceAudioWaveformStorage);
            /* a detached patient mgmt sop class is also ok */
            found = found || cmp(mediaSOPClassUID, UID_DetachedPatientManagementSOPClass);
        }
    }

    if (!found) {
        OFString sopClassName = dcmFindNameOfUID(mediaSOPClassUID.c_str());
        if (sopClassName.empty()) {
            sopClassName = mediaSOPClassUID;
        }
        CERR << "Error: invalid sop class (" << sopClassName
             << ") for " << getProfileName(dicomdirProfile) << " profile: " << fname << endl;
        /* give up checking */
        return OFFalse;
    }

    /*
    ** Is transfer syntax ok?
    */
    OFString transferSyntax = dcmFindString(m, DCM_TransferSyntaxUID);
    if (transferSyntax.empty()) {
        CERR << "Error: TransferSyntaxUID missing in meta-header: "
             << fname << endl;
        ok = OFFalse;
		
    }
    switch (dicomdirProfile)
    {
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
        case EDDP_CTandMR:
            if (cmp(mediaSOPClassUID, UID_DetachedPatientManagementSOPClass))
            {
                /* compare with expected transfer syntax */
                found = cmp(transferSyntax, expectedTransferSyntax);
                if (!found) {
                    OFString xferName = dcmFindNameOfUID(expectedTransferSyntax.c_str());
                    if (xferName.empty()) {
                        xferName = expectedTransferSyntax;
                    }
                    CERR << "Error: " << xferName << " expected: "
                         << fname << endl;
                    ok = OFFalse;
				}
            } else {
                /* need to check multiple transfer syntaxes */
                found = cmp(transferSyntax, UID_LittleEndianExplicitTransferSyntax) ||
                        cmp(transferSyntax, UID_JPEGProcess14SV1TransferSyntax);
                if (!found) {
                    OFString xferName1 = dcmFindNameOfUID(UID_LittleEndianExplicitTransferSyntax);
                    OFString xferName2 = dcmFindNameOfUID(UID_JPEGProcess14SV1TransferSyntax);
                    CERR << "Error: " << xferName1 << " or " << xferName2 << " expected: "
                         << fname << endl;
                    ok = OFFalse;
			    }
            }
            break;
        case EDDP_UltrasoundIDSF:
        case EDDP_UltrasoundSCSF:
        case EDDP_UltrasoundCCSF:
        case EDDP_UltrasoundIDMF:
        case EDDP_UltrasoundSCMF:
        case EDDP_UltrasoundCCMF:
            /* need to check multiple transfer syntaxes */
            found = cmp(transferSyntax, UID_LittleEndianExplicitTransferSyntax) ||
                    cmp(transferSyntax, UID_RLELossless) ||
                    cmp(transferSyntax, UID_JPEGProcess1TransferSyntax);
            if (!found) {
                OFString xferName1 = dcmFindNameOfUID(UID_LittleEndianExplicitTransferSyntax);
                OFString xferName2 = dcmFindNameOfUID(UID_RLELossless);
                OFString xferName3 = dcmFindNameOfUID(UID_JPEGProcess1TransferSyntax);
                CERR << "Error: " << xferName1 << ", " << xferName2 << " or " << xferName3 << " expected: "
                     << fname << endl;
                ok = OFFalse;
			}
            break;
#endif
        case EDDP_None:
            /* accept all transfer syntaxes */
            break;
        case EDDP_GeneralPurpose:
        default:
        {
            /* compare with expected transfer syntax */
            found = cmp(transferSyntax, expectedTransferSyntax);
            if (!found) {
                OFString xferName = dcmFindNameOfUID(expectedTransferSyntax.c_str());
                if (xferName.empty()) {
                    xferName = expectedTransferSyntax;
                }
                CERR << "Error: " << xferName << " expected: "
                     << fname << endl;
                ok = OFFalse;
			}
        }
    }

    /*
    ** are mandatory attributes for DICOMDIR available and valued?
    */

    /* PatientID is type 1 in DICOMDIR and type 2 in images. */
    if (!inventAttributes) {
        if (!checkExistsWithValue(d, DCM_PatientID, fname)) ok = OFFalse;
    }
    /* PatientsName is type 2 in DICOMDIR and images */
    if (!checkExists(d, DCM_PatientsName, fname)) ok = OFFalse;
    /* StudyDate is type 1 in DICOMDIR and type 2 in images */
    if (!inventAttributes) {
        if (!checkExistsWithValue(d, DCM_StudyDate, fname)) ok = OFFalse;
    }
    /* StudyTime is type 1 in DICOMDIR and type 2 in images */
    if (!inventAttributes) {
        if (!checkExistsWithValue(d, DCM_StudyTime, fname)) ok = OFFalse;
    }
    /* StudyDescription is type 2 in DICOMDIR and type 3 im images.
    ** We can create an empty attribute in the directory
    ** if (!checkExists(d, DCM_StudyDescription, fname)) ok = OFFalse;
    */
    /* StudyInstanceUID is type 1 in DICOMDIR and images */
    if (!checkExistsWithValue(d, DCM_StudyInstanceUID, fname)) ok = OFFalse;
    /* StudyID is type 1 in DICOMDIR and type 2 in images */
    if (!inventAttributes) {
        if (!checkExistsWithValue(d, DCM_StudyID, fname)) ok = OFFalse;
    }
    /* AccessionNumber is type 2 in DICOMDIR and type 3 in images
    ** We can create an empty attribute in the directory
    ** if (!checkExists(d, DCM_AccessionNumber, fname)) ok = OFFalse;
    */
    /* Modality is type 1 in DICOMDIR and type 1 in images */
    if (!checkExistsWithValue(d, DCM_Modality, fname)) ok = OFFalse;
    /* SeriesInstanceUID is type 1 in DICOMDIR and type 1 in images */
    if (!checkExistsWithValue(d, DCM_SeriesInstanceUID, fname)) ok = OFFalse;
    /* SeriesNumber is type 1 in DICOMDIR and type 2 in images */
    if (!inventAttributes) {
        if (!checkExistsWithValue(d, DCM_SeriesNumber, fname)) ok = OFFalse;
    }
    /* Image etc Numbers are type 1 in DICOMDIR but type 2 in images */
    if (cmp(mediaSOPClassUID, UID_StandaloneOverlayStorage)) {
        /* an overlay */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_OverlayNumber, fname))
                ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_StandaloneModalityLUTStorage)) {
        /* a modality lut */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_LookupTableNumber, fname))
                ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_StandaloneVOILUTStorage)) {
        /* a voi lut */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_LookupTableNumber, fname))
                ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_StandaloneCurveStorage) ||
               cmp(mediaSOPClassUID, UID_PETCurveStorage)) {
        /* a curve */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_CurveNumber, fname))
                ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_BasicTextSR) ||
               cmp(mediaSOPClassUID, UID_EnhancedSR) ||
               cmp(mediaSOPClassUID, UID_ComprehensiveSR) ||
               cmp(mediaSOPClassUID, UID_MammographyCADSR)) {
        /* a structured report */
        if (!checkExistsWithValue(d, DCM_InstanceNumber, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_CompletionFlag, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_VerificationFlag, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentDate, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentTime, fname)) ok = OFFalse;
        if (!dcmTagExistsWithValue(d, DCM_ConceptNameCodeSequence))
        {
            DcmTag cncsqtag(DCM_ConceptNameCodeSequence);
            CERR << "Error: required attribute " << cncsqtag.getTagName()
                 << " " << DCM_ConceptNameCodeSequence << " missing or empty in file: "
                 << fname << endl;
            ok = OFFalse;
        }
        OFString verificationFlag(dcmFindString(d, DCM_VerificationFlag));
        if (verificationFlag == "VERIFIED")
        {
            // VerificationDateTime is required if verificationFlag is VERIFIED
            // retrieve most recent (= last) entry from VerifyingObserverSequence
            DcmStack stack;
            if (d->search(DCM_VerifyingObserverSequence, stack, ESM_fromHere, OFFalse) == EC_Normal)
            {
                DcmSequenceOfItems *dseq = (DcmSequenceOfItems *)stack.top();
                if ((dseq != NULL) && (dseq->card() > 0))
                {
                    DcmItem *ditem = dseq->getItem(dseq->card() - 1);
                    if ((ditem == NULL) || (!checkExistsWithValue(ditem, DCM_VerificationDateTime, fname)))
                        ok = OFFalse;
                } else
                    ok = OFFalse;
            } else
                ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_GrayscaleSoftcopyPresentationStateStorage)) {
        /* a presentation state */
        if (!checkExistsWithValue(d, DCM_InstanceNumber, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_PresentationLabel, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_PresentationCreationDate, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_PresentationCreationTime, fname)) ok = OFFalse;
        if (!dcmTagExistsWithValue(d, DCM_ReferencedSeriesSequence))
        {
            CERR << "Error: required attribute ReferencedSeriesSequence "
                 << DCM_ReferencedSeriesSequence << " missing or empty in file: "
                 << fname << endl;
            ok = OFFalse;
        }
    } else if (cmp(mediaSOPClassUID, UID_TwelveLeadECGWaveformStorage) ||
               cmp(mediaSOPClassUID, UID_GeneralECGWaveformStorage) ||
               cmp(mediaSOPClassUID, UID_AmbulatoryECGWaveformStorage) ||
               cmp(mediaSOPClassUID, UID_HemodynamicWaveformStorage) ||
               cmp(mediaSOPClassUID, UID_CardiacElectrophysiologyWaveformStorage) ||
               cmp(mediaSOPClassUID, UID_BasicVoiceAudioWaveformStorage)) {
        /* a waveform */
        if (!checkExistsWithValue(d, DCM_InstanceNumber, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentDate, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentTime, fname)) ok = OFFalse;
    } else if (cmp(mediaSOPClassUID, UID_RTDoseStorage)) {
        /* a RT dose */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_InstanceNumber, fname))
                ok = OFFalse;
        }
        if (!checkExistsWithValue(d, DCM_DoseSummationType, fname)) ok = OFFalse;
    } else if (cmp(mediaSOPClassUID, UID_RTStructureSetStorage)) {
        /* a RT structure set */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_InstanceNumber, fname))
                ok = OFFalse;
        }
        if (!checkExistsWithValue(d, DCM_StructureSetLabel, fname)) ok = OFFalse;
    } else if (cmp(mediaSOPClassUID, UID_RTPlanStorage)) {
        /* a RT plan */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_InstanceNumber, fname))
                ok = OFFalse;
        }
        if (!checkExistsWithValue(d, DCM_RTPlanLabel, fname)) ok = OFFalse;
    } else if (cmp(mediaSOPClassUID, UID_RTBeamsTreatmentRecordStorage) ||
               cmp(mediaSOPClassUID, UID_RTBrachyTreatmentRecordStorage) ||
               cmp(mediaSOPClassUID, UID_RTTreatmentSummaryRecordStorage)) {
        /* a RT treat record */
        if (!checkExistsWithValue(d, DCM_InstanceNumber, fname)) ok = OFFalse;
    } else if (cmp(mediaSOPClassUID, UID_StoredPrintStorage)) {
        /* a stored print */
        /* (nothing to do) */
    } else if (cmp(mediaSOPClassUID, UID_KeyObjectSelectionDocument)) {
        /* a key object document */
        if (!checkExistsWithValue(d, DCM_InstanceNumber, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentDate, fname)) ok = OFFalse;
        if (!checkExistsWithValue(d, DCM_ContentTime, fname)) ok = OFFalse;
        if (!dcmTagExistsWithValue(d, DCM_ConceptNameCodeSequence))
        {
            CERR << "Error: required attribute ConceptNameCodeSequence "
                 << DCM_ConceptNameCodeSequence << " missing or empty in file: "
                 << fname << endl;
            ok = OFFalse;
        }
    } else {
        /* it can only be an image */
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
        if ((dicomdirProfile == EDDP_BasicCardiac) &&
             cmp(mediaSOPClassUID, UID_XRayAngiographicImageStorage)) {
            /* a XA image */
            if (!checkExistsWithStringValue(d, DCM_Modality, "XA", fname)) ok = OFFalse;
            if (!checkExistsWithMinMaxValue(d, DCM_Rows, 1, 512, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExistsWithMinMaxValue(d, DCM_Columns, 1, 512, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_BitsAllocated, 8, fname)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_BitsStored, 8, fname)) ok = OFFalse;
            if (cmp(dcmFindString(d, DCM_ImageType), "BIPLANE A") ||
                cmp(dcmFindString(d, DCM_ImageType), "BIPLANE B"))
            {
                CERR << "Error: BIPLANE images not allowed for " << getProfileName(dicomdirProfile)
                     << " profile: " << fname << endl;
            }
            /* overlay data, if present, shall be encoded in OverlayData (60XX,3000) */
            for (unsigned int grp = 0x6000; grp < 0x601f; grp += 2)
            {
                /* check minimum number of attributes required for an overlay plane to be displayed */
                if (dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayRows.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayColumns.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayBitsAllocated.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayBitPosition.getElement())) &&
                    !dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayData.getElement())))
                {
                    CERR << "Error: embedded overlay data present in group 0x" << hex << grp
                         << ", file: " << fname << endl;
                    ok = OFFalse;
                }
            }
        } else if ((dicomdirProfile == EDDP_XrayAngiographic) &&
                    cmp(mediaSOPClassUID, UID_XRayAngiographicImageStorage)) {
            /* a XA image */
            if (!checkExistsWithStringValue(d, DCM_Modality, "XA", fname)) ok = OFFalse;
            if (!checkExistsWithMinMaxValue(d, DCM_Rows, 1, 1024, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExistsWithMinMaxValue(d, DCM_Columns, 1, 1024, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExists(d, DCM_BitsStored, fname))
                ok = OFFalse;
            else {
                long bs = dcmFindInteger(d, DCM_BitsStored);
                if ((bs != 8) && (bs != 10) && (bs != 12))
                {
                    CERR << "Error: attribute BitsStored"
                         << " " << DCM_BitsStored << " has other value than expected in file: "
                         << fname << endl;
                    ok = OFFalse;
                }
            }
        } else if ((dicomdirProfile == EDDP_XrayAngiographic) &&
                    cmp(mediaSOPClassUID, UID_SecondaryCaptureImageStorage)) {
            /* a SC image */
            if (!checkExistsWithMinMaxValue(d, DCM_Rows, 1, 1024, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExistsWithMinMaxValue(d, DCM_Columns, 1, 1024, fname, resolutionCheck)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_SamplesPerPixel, 1, fname)) ok = OFFalse;
            if (!checkExistsWithStringValue(d, DCM_PhotometricInterpretation, "MONOCHROME2", fname)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_BitsAllocated, 8, fname)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_BitsStored, 8, fname)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_HighBit, 7, fname)) ok = OFFalse;
            if (!checkExistsWithIntegerValue(d, DCM_PixelRepresentation, 0, fname)) ok = OFFalse;
            /* check whether any overlay is present */
            for (unsigned int grp = 0x6000; grp < 0x601f; grp += 2)
            {
                /* check minimum number of attributes required for an overlay plane to be displayed */
                if (dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayRows.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayColumns.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayBitsAllocated.getElement())) &&
                    dcmTagExistsWithValue(d, DcmTagKey(grp, DCM_OverlayBitPosition.getElement())))
                {
                    CERR << "Error: overlay group 0x" << hex << grp
                         << "present in file: " << fname << endl;
                    ok = OFFalse;
                }
            }
        } else if ((dicomdirProfile == EDDP_CTandMR) &&
                    cmp(mediaSOPClassUID, UID_CTImageStorage)) {
            /* a CT image */
            if (!checkExistsWithStringValue(d, DCM_Modality, "CT", fname)) ok = OFFalse;
            if (!checkExistsWithStringValue(d, DCM_PhotometricInterpretation, "MONOCHROME2", fname)) ok = OFFalse;
        } else if ((dicomdirProfile == EDDP_CTandMR) &&
                    cmp(mediaSOPClassUID, UID_MRImageStorage)) {
            /* a MR image */
            if (!checkExistsWithStringValue(d, DCM_Modality, "MR", fname)) ok = OFFalse;
            if (!checkExistsWithStringValue(d, DCM_PhotometricInterpretation, "MONOCHROME2", fname)) ok = OFFalse;
            if (!checkExists(d, DCM_BitsStored, fname) || !checkExists(d, DCM_HighBit, fname))
                ok = OFFalse;
            else {
                long bs = dcmFindInteger(d, DCM_BitsStored);
                if ((bs != 8) && (bs != 12) && (bs != 16))
                {
                    CERR << "Error: attribute BitsStored"
                         << " " << DCM_BitsStored << " has other value than expected in file: "
                         << fname << endl;
                    ok = OFFalse;
                }
                long hb = dcmFindInteger(d, DCM_HighBit);
                if (hb != bs - 1)
                {
                   CERR << "Error: attribute HighBit"
                         << " " << DCM_HighBit << " has other value than expected in file: "
                         << fname << endl;
                    ok = OFFalse;
				}
            }
        } else if ((dicomdirProfile == EDDP_CTandMR) &&
                    cmp(mediaSOPClassUID, UID_SecondaryCaptureImageStorage)) {
            /* a SC image */
            if (!checkExistsWithIntegerValue(d, DCM_SamplesPerPixel, 1, fname)) ok = OFFalse;
            if (!checkExists(d, DCM_PhotometricInterpretation, fname))
                ok = OFFalse;
            else {
                OFString pi = dcmFindString(d, DCM_PhotometricInterpretation);
                if (cmp(pi, "MONOCHROME2"))
                {
                    if (!checkExists(d, DCM_BitsAllocated, fname) ||
                        !checkExists(d, DCM_BitsStored, fname) ||
                        !checkExists(d, DCM_HighBit, fname))
                    {
                        ok = OFFalse;
                    } else {
                        long ba = dcmFindInteger(d, DCM_BitsAllocated);
                        if ((ba != 8) && (ba != 16))
                        {
                            CERR << "Error: attribute BitsAllocated"
                                 << " " << DCM_BitsAllocated << " has other value than expected in file: "
                                 << fname << endl;
                            ok = OFFalse;
                        }
                        long bs = dcmFindInteger(d, DCM_BitsStored);
                        if (bs != ba)
                        {
                            CERR << "Error: attribute BitsStored"
                                 << " " << DCM_BitsStored << " has other value than expected in file: "
                                 << fname << endl;
                            ok = OFFalse;							
                        }
                        long hb = dcmFindInteger(d, DCM_HighBit);
                        if (hb != bs - 1)
                        {
                            CERR << "Error: attribute HighBit"
                                 << " " << DCM_HighBit << " has other value than expected in file: "
                                 << fname << endl;
                            ok = OFFalse;
                        }
                    }
                } else if (cmp(pi, "PALETTE COLOR"))
                {
                    if (!checkExistsWithIntegerValue(d, DCM_BitsAllocated, 8, fname)) ok = OFFalse;
                    if (!checkExistsWithIntegerValue(d, DCM_BitsStored, 8, fname)) ok = OFFalse;
                    if (!checkExistsWithIntegerValue(d, DCM_HighBit, 7, fname)) ok = OFFalse;
                } else {
                    CERR << "Error: attribute PhotometricInterpretation"
                         << " " << DCM_PhotometricInterpretation
                         << " has other value than expected in file: " << fname << endl;
                    ok = OFFalse;
                }
            }
        } else if ((dicomdirProfile == EDDP_UltrasoundIDSF) ||
                   (dicomdirProfile == EDDP_UltrasoundSCSF) ||
                   (dicomdirProfile == EDDP_UltrasoundCCSF) ||
                   (dicomdirProfile == EDDP_UltrasoundIDMF) ||
                   (dicomdirProfile == EDDP_UltrasoundSCMF) ||
                   (dicomdirProfile == EDDP_UltrasoundCCMF)) {
            /* a US image */
            if (!checkExists(d, DCM_PhotometricInterpretation, fname))
                ok = OFFalse;
            else {
                OFString pi = dcmFindString(d, DCM_PhotometricInterpretation);
                const OFBool uncompressed = cmp(transferSyntax, UID_LittleEndianExplicitTransferSyntax);
                const OFBool rle_lossless = cmp(transferSyntax, UID_RLELossless);
                const OFBool jpeg_lossy = cmp(transferSyntax, UID_JPEGProcess1TransferSyntax);
                OFBool valid = (cmp(pi, "MONOCHROME2") && (uncompressed || rle_lossless)) ||
                               (cmp(pi, "RGB") && (uncompressed || rle_lossless)) ||
                               (cmp(pi, "PALETTE COLOR") && (uncompressed || rle_lossless)) ||
                               (cmp(pi, "YBR_FULL") && rle_lossless) ||
                               (cmp(pi, "YBR_FULL_422") && (uncompressed || jpeg_lossy)) ||
                               (cmp(pi, "YBR_PARTIAL_422") && (uncompressed || jpeg_lossy));
                if (!valid) {
                    CERR << "Error: attribute PhotometricInterpretation"
                         << " " << DCM_PhotometricInterpretation
                         << " has other value than expected in file: " << fname << endl;
                    ok = OFFalse;
                }
            }
            if ((dicomdirProfile == EDDP_UltrasoundSCSF) || (dicomdirProfile == EDDP_UltrasoundCCSF) ||
                (dicomdirProfile == EDDP_UltrasoundSCMF) || (dicomdirProfile == EDDP_UltrasoundCCMF)) {
                /* check for US region calibration module (SC and CC profiles) */
                DcmStack stack;
                if (d->search(DCM_SequenceOfUltrasoundRegions, stack, ESM_fromHere, OFFalse) == EC_Normal)
                {
                    DcmSequenceOfItems *dseq = (DcmSequenceOfItems *)stack.top();
                    if ((dseq != NULL) && (dseq->card() > 0))
                    {
                        unsigned long i = 0;
                        const unsigned long count = dseq->card();
                        while (ok && (i < count))
                        {
                            DcmItem *ditem = dseq->getItem(i);
                            if (ditem != NULL)
                            {
                                if (!checkExistsWithValue(ditem, DCM_RegionLocationMinX0, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionLocationMinY0, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionLocationMaxX1, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionLocationMaxY1, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_PhysicalUnitsXDirection, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_PhysicalUnitsYDirection, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_PhysicalDeltaX, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_PhysicalDeltaY, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionSpatialFormat, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionDataType, fname)) ok = OFFalse;
                                if (!checkExistsWithValue(ditem, DCM_RegionFlags, fname)) ok = OFFalse;
                                if ((dicomdirProfile == EDDP_UltrasoundCCSF) ||
                                    (dicomdirProfile == EDDP_UltrasoundCCMF)) {
                                    /* check for pixel component organization (CC profile) */
                                    if (checkExists(ditem, DCM_PixelComponentOrganization, fname)) {
                                        if (!checkExistsWithValue(ditem, DCM_PixelComponentPhysicalUnits, fname)) ok = OFFalse;
                                        if (!checkExistsWithValue(ditem, DCM_PixelComponentDataType, fname)) ok = OFFalse;
                                        long pco = dcmFindInteger(d, DCM_PixelComponentOrganization);
                                        if (pco == 0) {
                                            /* pixel component organization: bit aligned positions */
                                            if (!checkExistsWithValue(ditem, DCM_PixelComponentMask, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_NumberOfTableBreakPoints, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfXBreakPoints, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfYBreakPoints, fname)) ok = OFFalse;
                                        } else if (pco == 1) {
                                            /* pixel component organization: ranges */
                                            if (!checkExistsWithValue(ditem, DCM_PixelComponentRangeStart, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_PixelComponentRangeStop, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_NumberOfTableBreakPoints, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfXBreakPoints, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfYBreakPoints, fname)) ok = OFFalse;
                                        } else if (pco == 2) {
                                            /* pixel component organization: table look up */
                                            if (!checkExistsWithValue(ditem, DCM_NumberOfTableEntries, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfPixelValues, fname)) ok = OFFalse;
                                            if (!checkExistsWithValue(ditem, DCM_TableOfParameterValues, fname)) ok = OFFalse;
                                        } else {
                                            CERR << "Error: attribute PixelComponentOrganization "
                                                 << DCM_PixelComponentOrganization << " has other value than expected in file: "
                                                 << fname << endl;
                                            ok = OFFalse;
                                        }
                                    } else
                                        ok = OFFalse;
                                }
                            } else
                                ok = OFFalse;
                            i++;
                        }
                    } else {
                        CERR << "Error: required attribute SequenceOfUltrasoundRegions "
                             << DCM_SequenceOfUltrasoundRegions << " empty in file: "
                             << fname << endl;
                        ok = OFFalse;
                    }
                } else {
                    DcmTag sqtag(DCM_SequenceOfUltrasoundRegions);
                    CERR << "Error: required attribute SequenceOfUltrasoundRegions "
                         << DCM_SequenceOfUltrasoundRegions << " missing in file: "
                         << fname << endl;
                    ok = OFFalse;
                }
            }
        }
#endif
        /* other images */
        if (!inventAttributes) {
            if (!checkExistsWithValue(d, DCM_InstanceNumber, fname))
                ok = OFFalse;
        }
    }

    return ok;
}

void DicomdirFactory::addConceptModContentItems(DcmItem* d, DcmDirectoryRecord* &rec)
{
    DcmStack stack;
    /* Content Sequence, type 1C (see DICOM 2000 part 3)
       "Contains the Target Content Items that modify the Concept Name
        Code Sequence of the root Content Item (Document Title).
        Required if the root Content Item is the Source Content Item of
        HAS CONCEPT MOD relationships."
    */
    if (d->search(DCM_ContentSequence, stack, ESM_fromHere, OFFalse) == EC_Normal)
    {
        DcmSequenceOfItems *dseq = (DcmSequenceOfItems *)stack.top();
        if (dseq != NULL)
        {
            const unsigned long count = dseq->card();
            if (count > 0)
            {
                /* create new ContenSequence */
                DcmSequenceOfItems *newseq = new DcmSequenceOfItems(DCM_ContentSequence);
                if (newseq != NULL)
                {
                    for (unsigned long i = 0; i < count; i++)
                    {
                         stack.clear();
                         DcmItem *ditem = dseq->getItem(i);
                         /* check RelationshipType */
                         if ((ditem != NULL) && (ditem->search(DCM_RelationshipType, stack, ESM_fromHere, OFFalse) == EC_Normal))
                         {
                             OFString str;
                             DcmCodeString *dstr = (DcmCodeString *)stack.top();
                             if ((dstr != NULL) && (dstr->getOFString(str, 0) == EC_Normal))
                             {
                                 if (str.compare("HAS CONCEPT MOD") == 0)
                                 {
                                     /* copy content item */
                                     DcmItem *newitem = new DcmItem(*ditem);
                                     if (newitem != NULL)
                                     {
                                         if (newseq->append(newitem) != EC_Normal)
                                             delete newitem;
                                     }
                                 }
                             }
                         }
                    }
                    if ((newseq->card() == 0) || (rec->insert(newseq, OFTrue /*replaceOld*/) != EC_Normal))
                      delete newseq;
                }
            }
        }
    }
}

DcmDirectoryRecord* DicomdirFactory::buildPatientRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Patient, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating patient record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_PatientID, d);
    dcmCopyString(rec, DCM_PatientsName, d);

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
    if ((dicomdirProfile == EDDP_BasicCardiac) || (dicomdirProfile == EDDP_XrayAngiographic))
    {
        /* additional type 2 keys specified by specific profiles */
        dcmCopyStringWithDefault(rec, DCM_PatientsBirthDate, d);
        dcmCopyStringWithDefault(rec, DCM_PatientsSex, d);
    }
#endif

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildStudyRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Study, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating study record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    if (dcmTagExistsWithValue(d, DCM_StudyDate)) {
        dcmCopyString(rec, DCM_StudyDate, d);
    } else {
        OFString altDate = alternativeStudyDate(d);
        CERR << "Warning: " << sourceFileName
             << ": StudyDate missing: using alternative: "
             << altDate << endl;
        dcmInsertString(rec, DCM_StudyDate, altDate);
    }
    if (dcmTagExistsWithValue(d, DCM_StudyTime)) {
        dcmCopyString(rec, DCM_StudyTime, d);
    } else {
        OFString altTime = alternativeStudyTime(d);
        CERR << "Warning: " << sourceFileName
             << ": StudyTime missing: using alternative: "
             << altTime << endl;
        dcmInsertString(rec, DCM_StudyTime, altTime);
    }
    dcmCopyString(rec, DCM_StudyDescription, d);
    dcmCopyString(rec, DCM_StudyInstanceUID, d);
    dcmCopyString(rec, DCM_StudyID, d);
    dcmCopyString(rec, DCM_AccessionNumber, d);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildSeriesRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Series, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating series record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_Modality, d);
    dcmCopyString(rec, DCM_SeriesInstanceUID, d);
    dcmCopyString(rec, DCM_SeriesNumber, d);

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
    if ((dicomdirProfile == EDDP_BasicCardiac) || (dicomdirProfile == EDDP_XrayAngiographic))
    {
        /* additional type 2 keys specified by specific profiles */
        dcmCopyStringWithDefault(rec, DCM_InstitutionName, d);
        dcmCopyStringWithDefault(rec, DCM_InstitutionAddress, d);
        dcmCopyStringWithDefault(rec, DCM_PerformingPhysiciansName, d);
    }
#endif

    return rec;
}


#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
OFBool DicomdirFactory::getExternalIcon(const OFString &filename, Uint8 *&pixel, const unsigned long count, const unsigned long width, const unsigned long height)
{
    OFBool result = OFFalse;
    FILE *file = fopen(filename.c_str(), "rb");
    if (file != NULL)
    {
        /* according to the pgm format no line should be longer than 70 characters */
        const int maxline = 256;
        char line[maxline], text[maxline];

        /* read magic number */
        if ((fgets(line, maxline, file) != NULL) && (sscanf(line, "%s", text) > 0) && (strcmp(text, "P5") == 0))
        {
            if ((fgets(line, maxline, file) != NULL) && (sscanf(line, "%s", text) > 0))
            {
                unsigned long pgmWidth, pgmHeight = 0;
                /* skip optional comment line and get width and height */
                if (((*text != '#') || (fgets(line, maxline, file) != NULL)) &&
                    (sscanf(line, "%lu %lu", &pgmWidth, &pgmHeight) > 0) && (pgmWidth > 0) && (pgmHeight > 0))
                {
                    unsigned int pgmMax = 0;
                    /* get maximum gray value */
                    if ((fgets(line, maxline, file) != NULL) && (sscanf(line, "%u", &pgmMax) > 0) && (pgmMax == 255))
                    {
                        const unsigned long pgmSize = pgmWidth * pgmHeight;
                        Uint8 *pgmData = new Uint8[pgmSize];
                        if (pgmData != NULL)
                        {
                            /* get pgm image data */
                            if (fread(pgmData, sizeof(Uint8), (size_t)pgmSize, file) == pgmSize)
                            {
                                /* if already scaled, just copy the bitmap */
                                if ((width == pgmWidth) && (height == pgmHeight) && (count == pgmSize))
                                {
                                    OFBitmanipTemplate<Uint8>::copyMem(pgmData, pixel, count);
                                } else {
                                    DiScaleTemplate<Uint8> scale(1, (Uint16)pgmWidth, (Uint16)pgmHeight, (Uint16)width, (Uint16)height, 1);
                                    scale.scaleData((const Uint8 **)&pgmData, &pixel, 1 /* interpolate */);
                                }
                                result = OFTrue;
                            }
                            delete[] pgmData;
                        } else
                            CERR << "Error: memory exhausted" << endl;
                    }
                }
            }
            if (!result)
                CERR << "Error: corrupt file format for external icon (not pgm binary)" << endl;
        } else
            CERR << "Error: wrong file format for external icon (pgm required)" << endl;
        fclose(file);
    } else
        CERR << "Error: cannot open file for external icon: " << filename << endl;
    return result;
}
OFBool DicomdirFactory::getIconFromDataset(DcmItem *d, Uint8 *&pixel, const unsigned long count, const unsigned long width, const unsigned long height)
{
    OFBool result = OFFalse;
    /* check buffer and size */
    if ((pixel != NULL) && (count >= width * height))
    {
        /* choose representitive frame */
        long fCount = dcmFindInteger(d, DCM_NumberOfFrames);
        long frame = dcmFindInteger(d, DCM_RepresentativeFrameNumber);
        if (fCount <= 0)
            fCount = 1;
        if (frame <= 0)
        {
            if (fCount > 3)
                frame = fCount / 3;     // recommended in PS3.11
            else
                frame = 1;
        } else if (frame > fCount)
            frame = fCount;
        /* optimization for compressed multiframe images */
        if (fCount > 1)
        {
            DcmStack stack;
            /* search for PixelData element on top-level */
            if (d->search(DCM_PixelData, stack, ESM_fromHere, OFFalse /* searchIntoSub */).good())
            {
                DcmPixelData *dpix = (DcmPixelData *)stack.top();
                if (dpix != NULL)
                {
                    /* get pixel data sequence (if available) */
                    E_TransferSyntax xfer = EXS_Unknown;
                    const DcmRepresentationParameter *rep = NULL;
                    dpix->getOriginalRepresentationKey(xfer, rep);
                    if ((xfer != EXS_Unknown) && DcmXfer(xfer).isEncapsulated())
                    {
                        DcmPixelSequence *pixSeq = NULL;
                        if (dpix->getEncapsulatedRepresentation(xfer, rep, pixSeq).good() && (pixSeq != NULL))
                        {
                            /* check whether each frame is stored in a separate pixel item */
                            if (pixSeq->card() == (unsigned long)(fCount + 1))
                            {
                                DcmPixelItem *pixItem = NULL;
                                long i;
                                /* delete all pixel items (apart from item #0) before ... */
                                for (i = 1; i < frame; i++)
                                {
                                    if (pixSeq->remove(pixItem, 1).good())
                                        delete pixItem;
                                }
                                /* ... and after representative frame/item */
                                for (i = frame; i < fCount; i++)
                                {
                                    if (pixSeq->remove(pixItem, 2).good())
                                        delete pixItem;
                                }
                                /* and finally, only 1 frame remains */
                                frame = 1;
                                fCount = 1;
                                /* adjust attributes in dataset to avoid errors in dcmimage */
                                d->putAndInsertString(DCM_NumberOfFrames, "1");
                                d->putAndInsertUint16(DCM_RepresentativeFrameNumber, 1);
                            }
                        }
                    }
                }
            }
        }
        /* open referenced image */
        DicomImage *image = new DicomImage(d, EXS_Unknown, 0 /* flags */, (unsigned long)(frame - 1) /* fstart */, 1 /* fcount */);
        if ((image != NULL) && (image->getStatus() == EIS_Normal))
        {
            /* check if image is monochrome */
            if (!image->isMonochrome())
            {
                /* ... if not create one */
                DicomImage *mono = image->createMonochromeImage();
                /* replace image by monochrome one */
                delete image;
                image = mono;
            }
            if (image != NULL)
            {
                /* create icon */
                DicomImage *scaled = image->createScaledImage(width, height, 1 /* interpolate */);
                if (scaled != NULL)
                {
                    /* set VOI window */
                    if (!scaled->setWindow(0))
                        scaled->setMinMaxWindow();
                    /* get pixel data */
                    void *data = (void *)pixel;
                    if (scaled->getOutputData(data, count, 8))
                        result = OFTrue;
                    delete scaled;
                }
            }
        }
        delete image;
    }
    return result;
}
#endif

DcmDirectoryRecord* DicomdirFactory::buildImageRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
		ERT_Image, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating image record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
    /* add monochrome icon image on IMAGE level */
    if (addIconImage)
    {
        const OFBool cardiac = (dicomdirProfile == EDDP_BasicCardiac) ||
                               (dicomdirProfile == EDDP_XrayAngiographic);
        /* Icon Image Sequence required for particular profiles */
        OFBool iconRequired = cardiac;
        OFBool ok = OFFalse;
        /* icon image size depends on application profile */
        const unsigned long width = (cardiac) ? 128 : 64;
        const unsigned long height = (cardiac) ? 128 : 64;
        DcmSequenceOfItems *dseq = new DcmSequenceOfItems(DCM_IconImageSequence);
        if (dseq != NULL)
        {
            DcmItem *ditem = new DcmItem();
            if (ditem != NULL)
            {
                /* Image Pixel Module */
                dcmInsertInteger(ditem, DCM_SamplesPerPixel, 1);
                dcmInsertString(ditem, DCM_PhotometricInterpretation, "MONOCHROME2");
                dcmInsertInteger(ditem, DCM_Rows, height);
                dcmInsertInteger(ditem, DCM_Columns, width);
                dcmInsertInteger(ditem, DCM_BitsAllocated, 8);
                dcmInsertInteger(ditem, DCM_BitsStored, 8);
                dcmInsertInteger(ditem, DCM_HighBit, 7);
                dcmInsertInteger(ditem, DCM_PixelRepresentation, 0);
                /* Pixel Data */
                const unsigned long pCount = width * height;
                Uint8 *pixel = new Uint8[pCount];
                if (pixel != NULL)
                {
                    OFBool iconOk = OFFalse;
                    /* prefix for external icons specified? */
                    if (iconPrefix.length() > 0)
                    {
                        OFString filename = iconPrefix + sourceFileName;
                        /* try to load external pgm icon */
                        iconOk = getExternalIcon(filename, pixel, pCount, width, height);
                    } else {
                        /* try to create icon from dataset */
                        iconOk = getIconFromDataset(d, pixel, pCount, width, height);
                        if (!iconOk)
                            CERR << "Warning: cannot create monochrome icon from image file, using default" << endl;
                    }
                    /* could not create icon so far: use default icon (if specified) */
                    if (!iconOk && (defaultIcon.length() > 0))
                        iconOk = getExternalIcon(defaultIcon, pixel, pCount, width, height);
                    /* default not available: use black image */
                    if (!iconOk)
                        OFBitmanipTemplate<Uint8>::zeroMem(pixel, pCount);
                    DcmPixelData *dpix = new DcmPixelData(DCM_PixelData);
                    if (dpix != NULL)
                    {
                        /* VR=OW is depreciated for Pixel Data */
                        dpix->setVR(EVR_OB);
                        if (dpix->putUint8Array(pixel, pCount) == EC_Normal)
                        {
                            if ((ditem->insert(dpix) == EC_Normal) && (dseq->insert(ditem) == EC_Normal) &&
                                (rec->insert(dseq, OFTrue /* replaceOld */) == EC_Normal))
                            {
                                ok = OFTrue;
                            }
                        } else
                            delete[] pixel;
                    }
                    if (!ok)
                        delete dpix;
                }
                if (!ok)
                    delete ditem;
            }
            if (!ok)
                delete dseq;
        }
        if (!ok)
        {
            if (iconRequired)
                CERR << "error";
            else
                CERR << "warning";
            CERR << ": cannot create IconImageSequence" << endl;
        }
        /* type 1C: required for XA images (type 1 for Basic Cardiac Profile) */
        if (cmp(dcmFindString(d, DCM_SOPClassUID), UID_XRayAngiographicImageStorage))
            dcmCopyString(rec, DCM_ImageType, d);
        else
            dcmCopyOptString(rec, DCM_ImageType, d);

        /* additional type 2 keys specified by specific profiles */
        dcmCopyStringWithDefault(rec, DCM_CalibrationImage, d);

        /* type 1C: required if ImageType is "BIPLANE A" or "BIPLANE B" */
        if (cmp(dcmFindString(d, DCM_ImageType), "BIPLANE A") ||
            cmp(dcmFindString(d, DCM_ImageType), "BIPLANE B"))
        {
            dcmCopySequence(rec, DCM_ReferencedImageSequence, d);
        } else
            dcmCopyOptSequence(rec, DCM_ReferencedImageSequence, d);
    }
    else if (dicomdirProfile == EDDP_CTandMR)
    {
        /* type 1 */
        dcmCopyInteger(rec, DCM_Rows, d);
        dcmCopyInteger(rec, DCM_Columns, d);
        /* type 1C */
        dcmCopyOptSequence(rec, DCM_ReferencedImageSequence, d);
        dcmCopyOptString(rec, DCM_ImagePositionPatient, d);
        dcmCopyOptString(rec, DCM_ImageOrientationPatient, d);
        dcmCopyOptString(rec, DCM_FrameOfReferenceUID, d);
        dcmCopyOptString(rec, DCM_PixelSpacing, d);
        /* tbd: may add optional Icon Image Sequence later on */
    } else
#endif
    {
        /* type 1C */
        dcmCopyOptString(rec, DCM_ImageType, d);
        dcmCopyOptSequence(rec, DCM_ReferencedImageSequence, d);
    }

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildOverlayRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Overlay, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating overlay record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_OverlayNumber, d);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildModalityLutRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_ModalityLut, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating modality lut record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_LookupTableNumber, d);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildVoiLutRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_VoiLut, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating voi lut record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_LookupTableNumber, d);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildCurveRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Curve, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating curve record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_CurveNumber, d);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildStructReportRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_StructReport, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating struct report record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_CompletionFlag, d);
    dcmCopyString(rec, DCM_VerificationFlag, d);
    dcmCopyString(rec, DCM_ContentDate, d);
    dcmCopyString(rec, DCM_ContentTime, d);
    OFString verificationFlag(dcmFindString(d, DCM_VerificationFlag));
    if (verificationFlag == "VERIFIED")
    {
      // VerificationDateTime is required if verificationFlag is VERIFIED
      // retrieve most recent (= last) entry from VerifyingObserverSequence
      DcmStack stack;
      if (d->search(DCM_VerifyingObserverSequence, stack, ESM_fromHere, OFFalse) == EC_Normal)
      {
        DcmSequenceOfItems *dseq = (DcmSequenceOfItems *)stack.top();
        if ((dseq != NULL) && (dseq->card() > 0))
        {
          DcmItem *ditem = dseq->getItem(dseq->card() - 1);
          if (ditem != NULL)
            dcmCopyString(rec, DCM_VerificationDateTime, ditem);
        }
      }
    }
    dcmCopySequence(rec, DCM_ConceptNameCodeSequence, d);
    addConceptModContentItems(d, rec);

    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildPresentationRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Presentation, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating presentation record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_PresentationLabel, d);
    dcmCopyString(rec, DCM_PresentationDescription, d);
    dcmCopyString(rec, DCM_PresentationCreationDate, d);
    dcmCopyString(rec, DCM_PresentationCreationTime, d);
    dcmCopyString(rec, DCM_PresentationCreatorsName, d);
    dcmCopySequence(rec, DCM_ReferencedSeriesSequence, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildWaveformRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_Waveform, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating waveform record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_ContentDate, d);
    dcmCopyString(rec, DCM_ContentTime, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildRTDoseRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_RTDose, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating rt dose record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_DoseSummationType, d);
    dcmCopyOptString(rec, DCM_DoseComment, d);
    dcmCopyOptSequence(rec, DCM_IconImageSequence, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildRTStructureSetRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_RTStructureSet, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating rt structure set record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_StructureSetLabel, d);
    dcmCopyString(rec, DCM_StructureSetDate, d);
    dcmCopyString(rec, DCM_StructureSetTime, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildRTPlanRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_RTPlan, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating rt plan record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_RTPlanLabel, d);
    dcmCopyString(rec, DCM_RTPlanDate, d);
    dcmCopyString(rec, DCM_RTPlanTime, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildRTTreatmentRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_RTTreatRecord, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating rt treat record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_TreatmentDate, d);
    dcmCopyString(rec, DCM_TreatmentTime, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildStoredPrintRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_StoredPrint, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating stored print record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyOptSequence(rec, DCM_IconImageSequence, d);
    return rec;
}

DcmDirectoryRecord* DicomdirFactory::buildKeyObjectDocRecord(const OFString& referencedFileName, DcmItem* d, const OFString& sourceFileName)
{
    DcmDirectoryRecord* rec = new DcmDirectoryRecord(
        ERT_KeyObjectDoc, referencedFileName.c_str(), sourceFileName.c_str());
    if (rec == NULL) {
        CERR << "Error: out of memory (creating key object doc record)" << endl;
        return NULL;
    }
    if (rec->error() != EC_Normal) {
        CERR << "Error: cannot create "
             << recordTypeToName(rec->getRecordType()) << " directory record: "
             << rec->error().text() << endl;
        delete rec;
        return NULL;
    }

    dcmCopyOptString(rec, DCM_SpecificCharacterSet, d);
    dcmCopyString(rec, DCM_InstanceNumber, d);
    dcmCopyString(rec, DCM_ContentDate, d);
    dcmCopyString(rec, DCM_ContentTime, d);
    dcmCopySequence(rec, DCM_ConceptNameCodeSequence, d);
    addConceptModContentItems(d, rec);

    return rec;
}

OFString DicomdirFactory::locateDicomFile(const OFString& fname)
{
    OFString fn = fname;
    dicomToHostFilename(fn);
    if (OFStandard::fileExists(fn)) {
        return fn;
    }
    /* trailing period */
    fn = fname + '.';
    dicomToHostFilename(fn);
    if (OFStandard::fileExists(fn)) {
        return fn;
    }
    /* lowercase */
    fn = fname;
    dicomToHostFilename(fn, OFTrue);
    if (OFStandard::fileExists(fn)) {
        return fn;
    }
    /* lowercase with trailing period */
    fn = fname + '.';
    dicomToHostFilename(fn, OFTrue);
    if (OFStandard::fileExists(fn)) {
        return fn;
    }

    return "";
}

OFBool DicomdirFactory::recordMatchesDataset(DcmDirectoryRecord *rec, DcmItem* dataset)
{
    OFBool match = OFFalse;

    switch (rec->getRecordType()) {
    case ERT_Patient:
        if (dcmTagExistsWithValue(dataset, DCM_PatientID)) {
            OFString patID = dcmFindString(rec, DCM_PatientID);
            /* PatientID is the primary key */
            match = cmp(patID, dcmFindString(dataset, DCM_PatientID));
            /* optional: check whether PatientsName also matches */
            if (match && !cmp(dcmFindString(rec, DCM_PatientsName),
                              dcmFindString(dataset, DCM_PatientsName)))
            {
                if (inventPatientID)
                {
                    CERR << "Warning: PatientsName inconsistant for PatientID: " << patID
                         << endl;
                    match = OFFalse;
                    /* remove current patient ID, will be replaced later */
                    dcmInsertString(dataset, DCM_PatientID, "");
                }
            }
        } else {
            /* if there is no value for PatientID in the dataset try using the
            ** PatientsName (also used if option "--match-patients-name" is set)
            */
            match = cmp(dcmFindString(rec, DCM_PatientsName),
                        dcmFindString(dataset, DCM_PatientsName));
        }
        break;
    case ERT_Study:
        if (dcmTagExists(rec, DCM_StudyInstanceUID)) {
            match = cmp(dcmFindString(rec, DCM_StudyInstanceUID),
                        dcmFindString(dataset, DCM_StudyInstanceUID));
        } else {
            /* the Study Instance UID can be in the referenced file instead */
            OFString reffname = dcmFindString(rec, DCM_ReferencedFileID);
            if (!reffname.empty()) {
                OFString fname = locateDicomFile(reffname);
                if (!fname.empty()) {
                    match = cmp(dcmFindStringInFile(fname,
                                                    DCM_StudyInstanceUID),
                                dcmFindString(dataset, DCM_StudyInstanceUID));
                } else {
                    CERR << "Error: cannot locate referenced file: "
                         << reffname << endl;
                }
            }
        }
        break;
    case ERT_Series:
        match = cmp(dcmFindString(rec, DCM_SeriesInstanceUID),
                    dcmFindString(dataset, DCM_SeriesInstanceUID));
        break;
    case ERT_Image:
    case ERT_Overlay:
    case ERT_Curve:
    case ERT_ModalityLut:
    case ERT_VoiLut:
    case ERT_StructReport:
    case ERT_Presentation:
    case ERT_Waveform:
    case ERT_RTDose:
    case ERT_RTStructureSet:
    case ERT_RTPlan:
    case ERT_RTTreatRecord:
    case ERT_StoredPrint:
    case ERT_KeyObjectDoc:
        /*
        ** The attribute ReferencedSOPInstanceUID is automatically
        ** put into a Directory Record when a filename is present.
        */
        match = cmp(dcmFindString(rec, DCM_ReferencedSOPInstanceUIDInFile),
                    dcmFindString(dataset, DCM_SOPInstanceUID));
        break;
    default:
        CERR << "Error: record type not yet implemented" << endl;
        return OFFalse;
        /* break; */  // never reached after return statement
    }

    return match;
}

DcmDirectoryRecord* DicomdirFactory::findExistingRecord(DcmDirectoryRecord *root, E_DirRecType dirtype, DcmItem* dataset)
{
    if (root == NULL) {
        return (DcmDirectoryRecord*)NULL;
    }

    DcmDirectoryRecord* r = NULL;
    unsigned long n = root->cardSub();
    OFBool found = OFFalse;
    for (unsigned long i=0; i<n && !found; i++) {
        r = root->getSub(i);
        if (dirtype == r->getRecordType()) {
            found = recordMatchesDataset(r, dataset);
        }
    }
    return (found)?(r):((DcmDirectoryRecord*)NULL);
}

DcmDirectoryRecord* DicomdirFactory::buildRecord(E_DirRecType dirtype, const OFString& referencedFileName, DcmItem* dataset, const OFString& sourceFileName)
{
    DcmDirectoryRecord *rec = NULL;

    switch (dirtype) {
    case ERT_Patient:
        rec = buildPatientRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Study:
        rec = buildStudyRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Series:
        rec = buildSeriesRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Image:
        rec = buildImageRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Overlay:
        rec = buildOverlayRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Curve:
        rec = buildCurveRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_ModalityLut:
        rec = buildModalityLutRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_VoiLut:
        rec = buildVoiLutRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_StructReport:
        rec = buildStructReportRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Presentation:
        rec = buildPresentationRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_Waveform:
        rec = buildWaveformRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_RTDose:
        rec = buildRTDoseRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_RTStructureSet:
        rec = buildRTStructureSetRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_RTPlan:
        rec = buildRTPlanRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_RTTreatRecord:
        rec = buildRTTreatmentRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_StoredPrint:
        rec = buildStoredPrintRecord(referencedFileName, dataset, sourceFileName);
        break;
    case ERT_KeyObjectDoc:
        rec = buildKeyObjectDocRecord(referencedFileName, dataset, sourceFileName);
        break;
    default:
        CERR << "Error: record type not yet implemented" << endl;
        return OFFalse;
        /* break; */  // never reached after return statement
    }
    return rec;
}

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
void DicomdirFactory::printIntegerAttribute(ostream& out, DcmTagKey& key, const long l)
{
    DcmTag tag(key);
    out << tag.getTagName() << " " << key << "=" << l;
}
#endif
void DicomdirFactory::printStringAttribute(ostream& out, DcmTagKey& key, const OFString& s)
{
    DcmTag tag(key);
    out << tag.getTagName() << " " << key << "=\"" << s << "\"";
}

void DicomdirFactory::printRecordUniqueKey(ostream& out, DcmDirectoryRecord *rec)
{
    switch (rec->getRecordType()) {
    case ERT_Patient:
        out << "PatientID " << DCM_PatientID << "=\""
            << dcmFindString(rec, DCM_PatientID) << "\"";
        break;
    case ERT_Study:
        out << "StudyInstanceUID " << DCM_StudyInstanceUID << "=\""
            << dcmFindString(rec, DCM_StudyInstanceUID) << "\"";
        break;
    case ERT_Series:
        out << "SeriesInstanceUID " << DCM_SeriesInstanceUID << "=\""
            << dcmFindString(rec, DCM_SeriesInstanceUID) << "\"";
        break;
    case ERT_Image:
    case ERT_Overlay:
    case ERT_Curve:
    case ERT_ModalityLut:
    case ERT_VoiLut:
    case ERT_StructReport:
    case ERT_Presentation:
    case ERT_Waveform:
    case ERT_RTDose:
    case ERT_RTStructureSet:
    case ERT_RTPlan:
    case ERT_RTTreatRecord:
    case ERT_StoredPrint:
    case ERT_KeyObjectDoc:
        out << "ReferencedSOPInstanceUIDInFile "
            << DCM_ReferencedSOPInstanceUIDInFile << "=\""
            << dcmFindString(rec, DCM_ReferencedSOPInstanceUIDInFile) << "\"";
        break;
    default:
        CERR << "Error: record type not yet implemented" << endl;
        break;
    }
}

#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
OFBool DicomdirFactory::compareIntegerAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName)
{
    long i1 = dcmFindInteger(rec, tag);
    long i2 = dcmFindInteger(dataset, tag);
    OFBool equals = (i1 == i2);

    if (!equals) {
        CERR << "Warning: file inconsistant with existing DICOMDIR record"
             << endl;
        CERR << "  " << recordTypeToName(rec->getRecordType())
             << " Record [Key: ";
        printRecordUniqueKey(CERR, rec);
        CERR << "]" << endl;
        CERR << "    Existing Record (origin: " << rec->getRecordsOriginFile()
             << ") defines: " << endl;
        CERR << "      ";
        printIntegerAttribute(CERR, tag, i1); CERR << endl;
        CERR << "    File (" << sourceFileName << ") defines:" << endl;
        CERR << "      ";
        printIntegerAttribute(CERR, tag, i2); CERR << endl;
    }

    return equals;
}
#endif
OFBool DicomdirFactory::compareStringAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName)
{
    OFString s1 = dcmFindString(rec, tag);
    OFString s2 = dcmFindString(dataset, tag);
    OFBool equals = cmp(s1, s2);

    if (!equals) {
        CERR << "Warning: file inconsistant with existing DICOMDIR record"
             << endl;
        CERR << "  " << recordTypeToName(rec->getRecordType())
             << " Record [Key: ";
        printRecordUniqueKey(CERR, rec);
        CERR << "]" << endl;
        CERR << "    Existing Record (origin: " << rec->getRecordsOriginFile()
             << ") defines: " << endl;
        CERR << "      ";
        printStringAttribute(CERR, tag, s1); CERR << endl;
        CERR << "    File (" << sourceFileName << ") defines:" << endl;
        CERR << "      ";
        printStringAttribute(CERR, tag, s2); CERR << endl;
    }

    return equals;
}


OFBool DicomdirFactory::areTagsEqual(DcmObject* obj1, DcmObject* obj2)
{
    if (obj1 == NULL || obj2 == NULL) {
        return OFFalse;
    }

    return obj1->getTag().getXTag() == obj2->getTag().getXTag();
}

OFString DicomdirFactory::constructTagName(DcmObject *obj)
{
    if (obj == NULL) {
        return "(NULL)";
    }
    DcmTag tag = obj->getTag();
    const char *tagName = tag.getTagName();

    // use tag name only if not equal to DcmTag_ERROR_TagName
    if (strcmp(tagName, DcmTag_ERROR_TagName)) return tagName;

    char buf[32];
    sprintf(buf, "(0x%04x,0x%04x)", tag.getGTag(), tag.getETag());
    return buf;
}

OFString DicomdirFactory::intToOFString(int i)
{
    char buf[64];
    sprintf(buf, "%d", i);
    return buf;
}

OFString DicomdirFactory::constructTagNameWithSQ(DcmObject *obj, DcmSequenceOfItems* fromSequence, int itemNumber)
{
    if (obj == NULL) {
        return "(NULL)";
    }
    OFString s;
    if (fromSequence != NULL) {
        s += constructTagName(fromSequence) + "[" + intToOFString(itemNumber) + "].";
    }
    s += constructTagName(obj);
    return s;
}

OFBool DicomdirFactory::compareBinaryValues(DcmElement* elem1, DcmElement* elem2, OFString& reason)
{
    Uint8* value1 = NULL;
    Uint8* value2 = NULL;
    Uint16* u16 = NULL;
    Sint16* s16 = NULL;
    Uint32* u32 = NULL;
    Sint32* s32 = NULL;
    Float32* f32 = NULL;
    Float64* f64 = NULL;


    OFCondition ec1 = EC_Normal;
    OFCondition ec2 = EC_Normal;

    switch (elem1->getVR()) {
    case EVR_OB:
        ec1 = elem1->getUint8Array(value1);
        ec2 = elem2->getUint8Array(value2);
        break;
    case EVR_AT:
    case EVR_US:
    case EVR_OW:
        ec1 = elem1->getUint16Array(u16);
        value1 = (Uint8*)u16;
        ec2 = elem2->getUint16Array(u16);
        value2 = (Uint8*)u16;
        break;
    case EVR_SS:
        ec1 = elem1->getSint16Array(s16);
        value1 = (Uint8*)s16;
        ec2 = elem2->getSint16Array(s16);
        value2 = (Uint8*)s16;
        break;
    case EVR_UL:
        ec1 = elem1->getUint32Array(u32);
        value1 = (Uint8*)u32;
        ec2 = elem2->getUint32Array(u32);
        value2 = (Uint8*)u32;
        break;
    case EVR_SL:
        ec1 = elem1->getSint32Array(s32);
        value1 = (Uint8*)s32;
        ec2 = elem2->getSint32Array(s32);
        value2 = (Uint8*)s32;
        break;
    case EVR_FL:
        ec1 = elem1->getFloat32Array(f32);
        value1 = (Uint8*)f32;
        ec2 = elem2->getFloat32Array(f32);
        value2 = (Uint8*)f32;
        break;
    case EVR_FD:
        ec1 = elem1->getFloat64Array(f64);
        value1 = (Uint8*)f64;
        ec2 = elem2->getFloat64Array(f64);
        value2 = (Uint8*)f64;
        break;
    /* currently cannot handle any other VR types */
    default:
        {
          DcmVR vr1(elem1->getVR());
          reason = "INTERNAL ERROR: Unexpected VR encountered: ";
          reason += vr1.getVRName();
          reason += "(" + constructTagName(elem1) + ")";
        }
        return OFFalse;
    }

    if (ec1 != EC_Normal || ec2 != EC_Normal)
    {
        DcmTag tag(elem1->getTag()); // create non const copy
        CERR << "dcmFindSequence: error while getting value of " << tag.getTagName()
             << " " << tag.getXTag() << ": "
             << ((ec1 != EC_Normal) ? ec1.text() : ec2.text()) << endl;
        reason = "cannot access binary value";
        return OFFalse;
    }

    Uint32 len = elem1->getLength();
    if (elem2->getLength() != len) return OFFalse;

    for (Uint32 i=0; i<len; i++) {
        if (value1[i] != value2[i]) {
            return OFFalse;
        }
    }
    return OFTrue;
}


OFBool DicomdirFactory::compareAttributes(DcmElement* elem1, DcmElement* elem2, DcmSequenceOfItems* fromSequence, int itemNumber, OFString& reason)
{
    if (elem1 == NULL && elem2 == NULL) {
        return OFTrue; /* nothing to compare */
    }
    if (elem1 == NULL) {
        reason = "missing attribute: " +  constructTagNameWithSQ(elem2, fromSequence, itemNumber);
        return OFFalse;
    }
    if (elem2 == NULL) {
        reason = "missing attribute: " +  constructTagNameWithSQ(elem1, fromSequence, itemNumber);
        return OFFalse;
    }

    /* do we have the same attributes? */
    if (!areTagsEqual(elem1, elem2)) {
        reason = "INTERNAL ERROR: different attributes: " +
            constructTagNameWithSQ(elem1, fromSequence, itemNumber) + " != " +
            constructTagNameWithSQ(elem2, fromSequence, itemNumber);
        return OFFalse;
    }

    /* are the VRs the same? */
    DcmVR vr1(elem1->getVR());
    DcmVR vr2(elem2->getVR());
    if (vr1.getEVR() != vr2.getEVR()) {
        reason = "different VRs: ";
        reason += vr1.getVRName() + OFString("!=") + vr2.getVRName();
        reason += "(" + constructTagNameWithSQ(elem1, fromSequence, itemNumber) + ")";
        return OFFalse;
    }

    /* are the lengths the same? */
    if (elem1->getLength() != elem2->getLength()) {
        reason = "different value lengths: " +
            intToOFString((int)(elem1->getLength())) + "!=" + intToOFString((int)(elem2->getLength())) +
            " (" + constructTagNameWithSQ(elem1, fromSequence, itemNumber) + ")";
        return OFFalse;
    }

    /* are the contents the same? */
    if (elem1->getLength() == 0) {
        return OFTrue;  /* no value */
    }

    if (elem1->isaString()) {
        OFString value1;
        OFString value2;

        elem1->getOFStringArray(value1);
        elem2->getOFStringArray(value2);
        if (value1 != value2) {
            reason = "different values: \"" + value1 + "\"!=\"" + value2 + "\" (" +
                constructTagNameWithSQ(elem1, fromSequence, itemNumber) + ")";
            return OFFalse;
        }

    } else if (vr1.getEVR() == EVR_SQ) {
        /* compare embedded sequences recursively */
        if (!compareSQAttributes((DcmSequenceOfItems*)elem1, (DcmSequenceOfItems*)elem2, reason)) {
            return OFFalse;
        }
    } else {
        /* compare binary values */
        if (!compareBinaryValues(elem1, elem2, reason)) {
            return OFFalse;
        }
    }

    /* the 2 attributes must be identical if we get this far */
    return OFTrue;
}

OFBool DicomdirFactory::compareItems(DcmItem* item1, DcmItem* item2, DcmSequenceOfItems* fromSequence, int itemNumber, OFString& reason)
{
    if (item1 == NULL && item2 == NULL) {
        return OFTrue; /* nothing to compare */
    }
    if (item1 == NULL || item2 == NULL) {
        reason = "missing item: " +  constructTagName(fromSequence) +
            "[" + intToOFString(itemNumber) + "]";
        return OFFalse;
    }


    int n1 = (int)(item1->card());
    int n2 = (int)(item2->card());

    if (n1 != n2) {
        reason = "different number attributes in items: " + constructTagName(fromSequence) +
            "[" + intToOFString(itemNumber) + "]";
        return OFFalse;
    }

    for (int i=0; i<n1; i++) {
        DcmElement* elem1 = item1->getElement(i);
        DcmElement* elem2 = item2->getElement(i);

        OFBool attributesAreEqual = compareAttributes(elem1, elem2, fromSequence, itemNumber, reason);
        if (!attributesAreEqual) {
            return OFFalse;
        }
    }

    /* the 2 items must be identical if we get this far */
    return OFTrue;
}


OFBool DicomdirFactory::compareSQAttributes(DcmSequenceOfItems* sq1, DcmSequenceOfItems* sq2, OFString& reason)
{
    if (sq1 == NULL && sq2 == NULL) {
        return OFTrue; /* nothing to compare */
    }
    if (sq1 == NULL) {
        reason = "missing sequence: " +  constructTagName(sq2);
        return OFFalse;
    }
    if (sq2 == NULL) {
        reason = "missing sequence: " +  constructTagName(sq1);
        return OFFalse;
    }

    if (!areTagsEqual(sq1, sq2)) {
        reason = "INTERNAL ERROR: different sequences: " +
            constructTagName(sq1) + " != " + constructTagName(sq2);
        return OFFalse;
    }

    int n1 = (int)(sq1->card());
    int n2 = (int)(sq2->card());

    if (n1 != n2) {
        reason = "different number of items in sequence: " + constructTagName(sq1);
        return OFFalse;
    }

    for (int i=0; i<n1; i++) {
        DcmItem *item1 = sq1->getItem(i);
        DcmItem *item2 = sq2->getItem(i);

        OFBool itemsAreEqual = compareItems(item1, item2, sq1, i, reason);
        if (!itemsAreEqual) {
            return OFFalse;
        }
    }
    /* the 2 sequences must be identical if we get this far */
    return OFTrue;
}

DcmSequenceOfItems* DicomdirFactory::dcmFindSequence(DcmItem* d, const DcmTagKey& key, OFBool searchIntoSub)
{
    DcmSequenceOfItems* sq = NULL;
    DcmElement *elem = NULL;
    DcmStack stack;
    OFCondition ec = EC_Normal;

    ec = d->search(key, stack, ESM_fromHere, searchIntoSub);
    elem = (DcmElement*) stack.top();

    if (ec != EC_Normal && ec != EC_TagNotFound) {
        DcmTag tag(key);
        CERR << "dcmFindSequence: error while finding " << tag.getTagName()
             << " " << key << ": "
             << ec.text() << endl;
    }

    if (elem && elem->ident() == EVR_SQ) {
        sq = (DcmSequenceOfItems*)elem;
    }
    return sq;
}

OFBool DicomdirFactory::compareSequenceAttributes(DcmTagKey& tag, DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName)
{
    DcmSequenceOfItems* sq1 = dcmFindSequence(rec, tag);
    DcmSequenceOfItems* sq2 = dcmFindSequence(dataset, tag);

    OFString reason;
    OFBool equals = compareSQAttributes(sq1, sq2, reason);

    if (!equals) {
        CERR << "Warning: file inconsistant with existing DICOMDIR record"
             << endl;
        CERR << "  " << recordTypeToName(rec->getRecordType())
             << " Record [Key: ";
        printRecordUniqueKey(CERR, rec);
        CERR << "]" << endl;
        CERR << "    Reason: " << reason << endl;;
        CERR << "    Existing Record (origin: " << rec->getRecordsOriginFile()
             << ") defines: " << endl;
        sq1->print(CERR, OFTrue, 2);
        CERR << "    File (" << sourceFileName << ") defines:" << endl;
        sq2->print(CERR, OFTrue, 2);
    }

    return equals;
}

OFBool DicomdirFactory::warnAboutInconsistantAttributes(DcmDirectoryRecord *rec, DcmItem* dataset, const OFString& sourceFileName, const OFBool abortCheck)
{
    /*
    ** See if all the attributes in rec match the values in dataset
    */
    OFBool ok = OFTrue;
    DcmElement *re = NULL;
    unsigned long n = rec->card();
    for (unsigned long i=0; i<n && (ok || !abortCheck); i++) {
        re = rec->getElement(i);
        if (re->getLength() > 0) {
            /* record attribute has a value */
            DcmTagKey tag = re->getTag().getXTag();
            if (dcmTagExistsWithValue(dataset, tag)) {
                /*
                ** We currently only handle strings, integers and sequences.
                ** This is not a huge problem since all the DICOMDIR
                ** attributes we generate are strings, integers or sequences.
                */
                if (re->isaString()) {
                    ok &= compareStringAttributes(tag, rec, dataset, sourceFileName);
#ifdef BUILD_DCMGPDIR_AS_DCMMKDIR
                } else if (re->getTag().getEVR() == EVR_US) {
                    /* might need to be extended to other VRs in the future */
                    ok &= compareIntegerAttributes(tag, rec, dataset, sourceFileName);
#endif
                } else if (re->getTag().getEVR() == EVR_SQ) {
                    /* do not check ContentSequence (see addConceptModContentItems()) */
                    if (re->getTag() != DCM_ContentSequence)
                        ok &= compareSequenceAttributes(tag, rec, dataset, sourceFileName);
                } else {
                    CERR << "INTERNAL ERROR: cannot compare: " << tag << endl;
                }
            }
        }
    }
    return !ok;
}

int DicomdirFactory::getISNumber(DcmItem *i, const DcmTagKey& key)
{
    int num = 0;
    OFString s = dcmFindString(i, key);
    if (!s.empty()) {
        sscanf(s.c_str(), "%d", &num);
    }
    return num;
}

OFCondition DicomdirFactory::insertWithISCriterion(DcmDirectoryRecord *parent, DcmDirectoryRecord *child, const DcmTagKey& criterionKey)
{
    /*
    ** Insert the child directory record into the parent's list based
    ** on the numeric value of a VR=IS criterionKey attribute.
    ** Strange things will happen if criterionKey does not represent VR=IS.
    */

    OFCondition cond = EC_Normal;
    int number = getISNumber(child, criterionKey);
    int pos = 0;
    int count = (int)(parent->cardSub());
    OFBool found = OFFalse;
    if (count > 0) {
        for (int i=0; i<count && !found; i++) {
            DcmDirectoryRecord* rec = parent->getSub(i);
            int curNumber = getISNumber(rec, criterionKey);
            if (curNumber > number) {
                found = OFTrue;
            }
            pos = i;
        }
    }
    if (pos < (count-1)) {
        cond = parent->insertSub(child, pos, OFTrue);
    } else {
        cond = parent->insertSub(child); /* append */
    }
    return cond;
}

OFCondition DicomdirFactory::insertSortedUnder(DcmDirectoryRecord *parent, DcmDirectoryRecord *child)
{
    OFCondition cond = EC_Normal;
    switch (child->getRecordType()) {
      case ERT_Image:
        /* try to insert based on Image/InstanceNumber */
        cond = insertWithISCriterion(parent, child, DCM_InstanceNumber);
        break;
      case ERT_Overlay:
        /* try to insert based on OverlayNumber */
        cond = insertWithISCriterion(parent, child, DCM_OverlayNumber);
        break;
      case ERT_Curve:
        /* try to insert based on CurveNumber */
        cond = insertWithISCriterion(parent, child, DCM_CurveNumber);
        break;
      case ERT_ModalityLut:
      case ERT_VoiLut:
        /* try to insert based on LUTNumber */
        cond = insertWithISCriterion(parent, child, DCM_LookupTableNumber);
        break;
      case ERT_StructReport:
      case ERT_Presentation:
      case ERT_Waveform:
      case ERT_RTDose:
      case ERT_RTStructureSet:
      case ERT_RTPlan:
      case ERT_RTTreatRecord:
      case ERT_StoredPrint:
      case ERT_KeyObjectDoc:
        /* try to insert based on InstanceNumber */
        cond = insertWithISCriterion(parent, child, DCM_InstanceNumber);
        break;
      case ERT_Series:
        /* try to insert based on SeriesNumber */
        cond = insertWithISCriterion(parent, child, DCM_SeriesNumber);
        break;
      default:
        /* append */
        cond = parent->insertSub(child);
        break;
    }
    return cond;
}

DcmDirectoryRecord* DicomdirFactory::includeRecord(DcmDirectoryRecord *parentRec, E_DirRecType dirtype, DcmItem* dataset, const OFString& referencedFileName, const OFString& sourceFileName)
{
    DcmDirectoryRecord *rec =
        findExistingRecord(parentRec, dirtype, dataset);
    if (rec == NULL) {
        rec = buildRecord(dirtype, referencedFileName,
                          dataset, sourceFileName);
        if (rec != NULL) {
            /* insert underneath correct parent record */
            OFCondition cond = insertSortedUnder(parentRec, rec);
            if (cond != EC_Normal) {
                CERR << "Error: " << cond.text()
                     << ": cannot insert " << recordTypeToName(dirtype)
                     << " record" << endl;
                return NULL;
            }
        }
    }
    return rec;
}

OFBool DicomdirFactory::addToDir(DcmDirectoryRecord* rootRec, const OFString& ifname)
{
    OFString fname = ifname;
	//OFString fname = "Hello_again_and_again";

    if (verbosemode) {
        COUT << "adding: " << fname << endl;
    }

    DcmFileFormat ff;
    OFCondition cond = ff.loadFile(fname.c_str());

    if (cond.bad()) {
        CERR << "Error: " << cond.text()
             << ": reading file: " << fname << endl;
        return OFFalse;
    }

    /*
    ** Massage filename into dicom format
    ** (dos conventions for path separators)
    */
    hostToDicomFilename(fname);

    DcmMetaInfo* metainfo = ff.getMetaInfo();
    DcmDataset* dataset = ff.getDataset();
    /* what kind of object (SOP Class) is stored in the file */
    OFString sopClass = dcmFindString(metainfo, DCM_MediaStorageSOPClassUID);

    /*
    ** Build the necessary dicomdir records
    */

    /*
    ** Add a patient record underneath the root
    */

    OFString emptyString;
    DcmDirectoryRecord *patientRec =
        includeRecord(rootRec, ERT_Patient, dataset, emptyString, ifname);
    if (patientRec == NULL) {
        return OFFalse;
    }
    if (patientRec->getRecordsOriginFile() == NULL) {
        // not yet noted
        patientRec->setRecordsOriginFile(ifname.c_str());
    }
    /* abort on any inconsistancy */
    if (warnAboutInconsistantAttributes(patientRec, dataset, ifname, abortInconsistFile) && abortInconsistFile) {
        CERR << "Error: aborting on first inconsistant file" << endl;
        return OFFalse;
    }

    /* if PatientMgmgt file then attach it to Patient Record and stop */
    if (cmp(sopClass, UID_DetachedPatientManagementMetaSOPClass)) {
        cond = patientRec->assignToSOPFile(fname.c_str(), ifname.c_str());
        if (cond != EC_Normal) {
            CERR << "Error: " << cond.text()
                 << ": cannot assign patient record to file: "
                 << fname << endl;
            return OFFalse;
        }
        /* stop adding STUDY/SERIES/.. records */
        return OFTrue;
    }

    /*
    ** Add a study record underneath the actual patient record
    */

    DcmDirectoryRecord *studyRec =
        includeRecord(patientRec, ERT_Study, dataset, emptyString, ifname);
    if (studyRec == NULL) {
        return OFFalse;
    }
    if (studyRec->getRecordsOriginFile() == NULL) {
        // not yet noted
        studyRec->setRecordsOriginFile(ifname.c_str());
    }
    /* abort on any inconsistancy */
    if (warnAboutInconsistantAttributes(studyRec, dataset, ifname, abortInconsistFile) && abortInconsistFile) {
        CERR << "Error: aborting on first inconsistant file" << endl;
        return OFFalse;
    }

    /*
    ** Add a series record underneath the actual study
    */
    DcmDirectoryRecord *seriesRec =
        includeRecord(studyRec, ERT_Series, dataset, emptyString, ifname);
    if (seriesRec == NULL) {
        return OFFalse;
    }
    if (seriesRec->getRecordsOriginFile() == NULL) {
        // not yet noted
        seriesRec->setRecordsOriginFile(ifname.c_str());
    }
    /* abort on any inconsistancy */
    if (warnAboutInconsistantAttributes(seriesRec, dataset, ifname, abortInconsistFile) && abortInconsistFile) {
        CERR << "Error: aborting on first inconsistant file" << endl;
        return OFFalse;
    }

    /*
    ** Add one of the image-like records underneath the actual series record
    */
    DcmDirectoryRecord *rec = NULL;

    if (cmp(sopClass, UID_StandaloneOverlayStorage)) {
        /* Add an overlay record */
        rec = includeRecord(seriesRec, ERT_Overlay, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_StandaloneModalityLUTStorage)) {
        /* Add a modality lut record */
        rec = includeRecord(seriesRec, ERT_ModalityLut, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_StandaloneVOILUTStorage)) {
        /* Add a voi lut record */
        rec = includeRecord(seriesRec, ERT_VoiLut, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_StandaloneCurveStorage) ||
               cmp(sopClass, UID_PETCurveStorage)) {
        /* Add a curve record */
        rec = includeRecord(seriesRec, ERT_Curve, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_BasicTextSR) ||
               cmp(sopClass, UID_EnhancedSR) ||
               cmp(sopClass, UID_ComprehensiveSR) ||
               cmp(sopClass, UID_MammographyCADSR)) {
        /* Add a structured report */
        rec = includeRecord(seriesRec, ERT_StructReport, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_GrayscaleSoftcopyPresentationStateStorage)) {
        /* Add a presentation state */
        rec = includeRecord(seriesRec, ERT_Presentation, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_TwelveLeadECGWaveformStorage) ||
               cmp(sopClass, UID_GeneralECGWaveformStorage) ||
               cmp(sopClass, UID_AmbulatoryECGWaveformStorage) ||
               cmp(sopClass, UID_HemodynamicWaveformStorage) ||
               cmp(sopClass, UID_CardiacElectrophysiologyWaveformStorage) ||
               cmp(sopClass, UID_BasicVoiceAudioWaveformStorage)) {
        /* Add a waveform */
        rec = includeRecord(seriesRec, ERT_Waveform, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_RTDoseStorage)) {
        /* Add a RT dose */
        rec = includeRecord(seriesRec, ERT_RTDose, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_RTStructureSetStorage)) {
        /* Add a RT structure set */
        rec = includeRecord(seriesRec, ERT_RTStructureSet, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_RTPlanStorage)) {
        /* Add a RT plan */
        rec = includeRecord(seriesRec, ERT_RTPlan, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_RTBeamsTreatmentRecordStorage) ||
               cmp(sopClass, UID_RTBrachyTreatmentRecordStorage) ||
               cmp(sopClass, UID_RTTreatmentSummaryRecordStorage)) {
        /* Add a RT treat record */
        rec = includeRecord(seriesRec, ERT_RTTreatRecord, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_StoredPrintStorage)) {
        /* Add a stored print */
        rec = includeRecord(seriesRec, ERT_StoredPrint, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else if (cmp(sopClass, UID_KeyObjectSelectionDocument)) {
        /* Add a key object document */
        rec = includeRecord(seriesRec, ERT_KeyObjectDoc, dataset, fname, ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    } else {
        /* it can only be an image */
        /* Add an image record */
        //rec = includeRecord(seriesRec, ERT_Image, dataset, fname, ifname);
		std::string absFileName = ifname.c_str();
		std::string pure_filename = absFileName.substr(path.length()+1, absFileName.length());
		rec = includeRecord(seriesRec, ERT_Image, dataset, pure_filename.c_str(), ifname);
        if (rec == NULL) {
            return OFFalse;
        }
    }
    if (rec->getRecordsOriginFile() == NULL) {
        // not yet noted
        rec->setRecordsOriginFile(ifname.c_str());
    }
    /* abort on any inconsistancy */
    if (warnAboutInconsistantAttributes(rec, dataset, ifname, abortInconsistFile) && abortInconsistFile) {
        CERR << "Error: aborting on first inconsistant file" << endl;
        return OFFalse;
    }

    return OFTrue;
}

OFBool DicomdirFactory::areCSCharsValid(const OFString& fname)
{
    int len = fname.size();
    for (int i=0; i<len; i++) {
        char c = fname[i];
        if ((c != ' ') && (c != '_') && !isdigit(c) &&
            !(isalpha(c) && isupper(c))) {
            CERR << "Error: invalid character encountered: "
                 << char(c) << endl;
            return OFFalse;
        }
    }
    return OFTrue;
}

OFString DicomdirFactory::locateInvalidFileNameChars(const OFString& fname, char separator)
{
    int len = fname.size();
    if (mapFilenames && fname[len-1] == '.') {
        /* disregard trailing point */
        len--;
    }
    OFString invalidChars;
    for (int i=0; i<len; i++) {
        char c = fname[i];
        if ((c == '_') || isdigit(c) || (c == separator) ||
            (isalpha(c) && (isupper(c) || (islower(c) && mapFilenames)))) {
            /* all ok */
        } else if (c == '\\') {
            /* this is only ok if the underlying OS uses \ */
            if (separator != '\\') {
                if (invalidChars.find(c) == OFString_npos) {
                    invalidChars += c;
                }
            }
        } else if (isalpha(c) && islower(c)) {
            if (invalidChars.find(c) == OFString_npos) {
                invalidChars += c;
            }
        } else {
            if (invalidChars.find(c) == OFString_npos) {
                invalidChars += c;
            }
        }
    }
    return invalidChars;
}

OFBool DicomdirFactory::isaValidFileName(const OFString& fname, char sep)
{
    OFBool ok = OFTrue;
    if (fname.length() == 0) {
        CERR << "Error: <empty string> not allowed as filename" << endl;
        ok = OFFalse;
    }
    /*
    ** Is the file name path ok?
    ** The filename is assumed to be in local format for this check.
    */
    OFString invalidChars = locateInvalidFileNameChars(fname, sep);
    if (!invalidChars.empty()) {
        CERR << "Error: invalid characters (\"" << invalidChars
             << "\") in filename: " << fname << endl;
        ok = OFFalse;
    }
    /*
    ** Ensure that the max number of components is not being exceeded
    */
    if (componentCount(fname, sep) > MAX_FNAME_COMPONENTS) {
        CERR << "Error: too many path components (max "
             << MAX_FNAME_COMPONENTS << ") in filename: "
             << fname << endl;
        ok = OFFalse;
    }
    /*
    ** Ensure that each component is not too large
    */
    if (isComponentTooLarge(fname, MAX_FNAME_COMPONENT_SIZE, sep)) {
        CERR << "Error: component too large (max "
             << MAX_FNAME_COMPONENT_SIZE << " characters) in filename: "
             << fname << endl;
        ok = OFFalse;
    }
    return ok;
}

OFBool DicomdirFactory::isaValidFileSetID(const OFString& aFsid)
{
    OFBool ok = OFTrue;
    if (aFsid.empty()) {
        return OFFalse;
    }
    /*
    ** Are the characters ok?
    */
    if (!areCSCharsValid(aFsid)) {
        CERR << "       invalid characters in FileSetID: "
             << aFsid << endl;
        ok = OFFalse;
    }
    /*
    ** Ensure that the max number of components is not being exceeded
    */
    if (componentCount(aFsid) != 1) {
        CERR << "Error: too many components in FileSetID: "
             << aFsid << endl;
        ok = OFFalse;
    }
    /*
    ** Ensure that each component is not too large
    */
    DcmVR cs(EVR_CS);
    if (isComponentTooLarge(aFsid, (int)(cs.getMaxValueLength()))) {
        CERR << "Error: too large: " << aFsid << endl;
        ok = OFFalse;
    }
    return ok;
}


OFBool DicomdirFactory::checkFileCanBeUsed(const OFString& fname)
{
    if (verbosemode) {
        COUT << "checking: " << fname << endl;
    }

    //if (!isaValidFileName(fname)) return OFFalse;

    /*
    ** Does the file exist??
    */

    DcmFileFormat ff;
    OFCondition cond = ff.loadFile(fname.c_str());

	// already checked in DicomImporter
    //if (cond.bad()) {
    //    CERR << "Error: "
    //         << cond.text()
    //         << ": reading file: " << fname << endl;
    //    /* cannot continue checking */
    //    return OFFalse;
    //}

    return checkImage(fname, &ff);
}

void DicomdirFactory::inventMissingImageLevelAttributes(DcmDirectoryRecord *parent)
{
    int imageNumber = 1;
    int overlayNumber = 1;
    int lutNumber = 1;
    int curveNumber = 1;
    int rtDoseInstanceNumber = 1;
    int rtStructureSetInstanceNumber = 1;
    int rtPlanInstanceNumber = 1;
    int storedPrintInstanceNumber = 1;

    int count = (int)(parent->cardSub());
    for (int i=0; i<count; i++) {
        DcmDirectoryRecord* rec = parent->getSub(i);

        switch (rec->getRecordType()) {
        case ERT_Image:
            if (!dcmTagExistsWithValue(rec, DCM_InstanceNumber)) {
                OFString defNum = defaultNumber(imageNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing ImageNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_InstanceNumber, defNum);
            }
            break;
        case ERT_Overlay:
            if (!dcmTagExistsWithValue(rec, DCM_OverlayNumber)) {
                OFString defNum = defaultNumber(overlayNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing OverlayNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_OverlayNumber, defNum);
            }
            break;
        case ERT_ModalityLut:
        case ERT_VoiLut:
            if (!dcmTagExistsWithValue(rec, DCM_LookupTableNumber)) {
                OFString defNum = defaultNumber(lutNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing LutNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_LookupTableNumber, defNum);
            }
            break;
        case ERT_Curve:
            if (!dcmTagExistsWithValue(rec, DCM_CurveNumber)) {
                OFString defNum = defaultNumber(curveNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing CurveNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_CurveNumber, defNum);
            }
            break;
        case ERT_RTDose:
            if (!dcmTagExistsWithValue(rec, DCM_InstanceNumber)) {
                OFString defNum = defaultNumber(rtDoseInstanceNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing InstanceNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_InstanceNumber, defNum);
            }
            break;
        case ERT_RTStructureSet:
            if (!dcmTagExistsWithValue(rec, DCM_InstanceNumber)) {
                OFString defNum = defaultNumber(rtStructureSetInstanceNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing InstanceNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_InstanceNumber, defNum);
            }
            break;
        case ERT_RTPlan:
            if (!dcmTagExistsWithValue(rec, DCM_InstanceNumber)) {
                OFString defNum = defaultNumber(rtPlanInstanceNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing InstanceNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_InstanceNumber, defNum);
            }
            break;
        case ERT_StoredPrint:
            if (!dcmTagExistsWithValue(rec, DCM_InstanceNumber)) {
                OFString defNum = defaultNumber(storedPrintInstanceNumber++);
                CERR << "Warning: " <<  recordTypeToName(rec->getRecordType())
                     << "Record (origin: " << rec->getRecordsOriginFile()
                     << ") inventing InstanceNumber: "
                     << defNum << endl;
                dcmInsertString(rec, DCM_InstanceNumber, defNum);
            }
            break;
        case ERT_StructReport:
        case ERT_Presentation:
        case ERT_Waveform:
        case ERT_RTTreatRecord:
        case ERT_KeyObjectDoc:
            /* nothing to do */
            break;
        default:
            CERR << "Error: (INTERNAL): inventMissingImageLevelAttributes: "
                 << "encountered unexpected record: "
                 << recordTypeToName(rec->getRecordType())
                 << endl;
            break;
        }
    }
}

void DicomdirFactory::inventMissingSeriesLevelAttributes(DcmDirectoryRecord *parent)
{
    int seriesNumber = 0;
    int count = (int)(parent->cardSub());
    for (int i=0; i<count; i++) {
        DcmDirectoryRecord* rec = parent->getSub(i);
        if (!dcmTagExistsWithValue(rec, DCM_SeriesNumber)) {
            OFString defNum = defaultNumber(seriesNumber++);
            CERR << "Warning: Series Record (origin: "
                 << rec->getRecordsOriginFile() << ") inventing SeriesNumber: "
                 << defNum << endl;
            dcmInsertString(rec, DCM_SeriesNumber, defNum);
        }
        inventMissingImageLevelAttributes(rec);
    }

}

void DicomdirFactory::inventMissingStudyLevelAttributes(DcmDirectoryRecord *parent)
{
    static int studyNumber = 0; /* make invented StudyID global */

    int count = (int)(parent->cardSub());
    for (int i=0; i<count; i++) {
        DcmDirectoryRecord* rec = parent->getSub(i);
        if (!dcmTagExistsWithValue(rec, DCM_StudyID)) {
            OFString defId = defaultID("DCMTKSTUDY", studyNumber++);
            CERR << "Warning: Study Record (origin: "
                 << rec->getRecordsOriginFile() << ") inventing StudyID: "
                 << defId << endl;
            dcmInsertString(rec, DCM_StudyID, defId);
        }
        inventMissingSeriesLevelAttributes(rec);
    }
}

void DicomdirFactory::inventMissingAttributes(DcmDirectoryRecord *root, const OFBool recurse)
{
    static int patientNumber = 0; /* make invented PatientID global */

    int count = (int)(root->cardSub());
    for (int i=0; i<count; i++) {
        DcmDirectoryRecord* rec = root->getSub(i);
        if (!dcmTagExistsWithValue(rec, DCM_PatientID)) {
            OFString defId = defaultID("DCMTKPAT", patientNumber++);
            CERR << "Warning: Patient Record (origin: "
                 << rec->getRecordsOriginFile() << ") inventing PatientID: "
                 << defId << endl;
            dcmInsertString(rec, DCM_PatientID, defId);
        }
        if (recurse)
            inventMissingStudyLevelAttributes(rec);
    }
}

OFBool DicomdirFactory::copyFile(const OFString& fromfname, const OFString& tofname)
{
    FILE* fromf = NULL;
    FILE* tof = NULL;
    if ((fromf = fopen(fromfname.c_str(), "rb")) == NULL) {
        CERR << "Error: copying files: cannot open: " << fromfname << endl;
        return OFFalse;
    }
    if ((tof = fopen(tofname.c_str(), "wb")) == NULL) {
        CERR << "Error: copying files: cannot create: " << tofname << endl;
        fclose(fromf);
        return OFFalse;
    }
    OFBool ok = OFTrue;
    int c = 0;
    while ((c = getc(fromf)) != EOF) {
        if (putc(c, tof) == EOF) {
            CERR << "Error: copying files: " << fromfname << " to "
                 << tofname << endl;
            ok = OFFalse;
            break;
        }
    }
    fclose(fromf);
    fclose(tof);
    return ok;
}

DcmDicomDir* DicomdirFactory::createDicomdirFromFiles(OFList<OFString>& fileNames)
{
    OFBool ok = OFTrue;

    /*
    ** Check the FileSetID and the FileSetDescriptorFileID
    */
    if (!isaValidFileSetID(fsid)) {
        CERR << "Error: invalid FileSetID: "
             << fsdfid << endl;
        ok = OFFalse;
    }

    if (!fsdfid.empty() && !isaValidFileName(fsdfid)) {
        CERR << "       bad FileSetDescriptorFileID: "
             << fsdfid << endl;
        ok = OFFalse;
    }

    if (!fsdfid.empty() && !OFStandard::fileExists(fsdfid)) {
        CERR << "Error: cannot find FileSetDescriptorFileID: "
             << fsdfid << endl;
        ok = OFFalse;
    }

    /*
    ** Check the files before adding
    */

    OFList<OFString> goodFileNames;
    OFList<OFString> badFileNames;
    OFListIterator(OFString) iter = fileNames.begin();
    while (iter != fileNames.end()) {
        if (checkFileCanBeUsed(*iter)) {
            goodFileNames.push_back(*iter);
        } else {
            badFileNames.push_back(*iter);;
        }
        ++iter;
    }
    if (goodFileNames.empty()) {
        CERR << "FATAL ERROR: no good files: DICOMDIR not created" << endl;
        return OFFalse;
    } else if (!badFileNames.empty()) {
        CERR << "WARNING: " << badFileNames.size()
             << " file(s) cannot be added to DICOMDIR: " << endl;
        iter = badFileNames.begin();
        while (iter != badFileNames.end()) {
            CERR << "         " << *iter << endl;
            ++iter;
        }
    }

    /*
    ** Create the DICOMDIR object
    */

	
    std::stringstream tmp;
	tmp << path << "/" << ofname;
	DcmDicomDir *dicomdir = new DcmDicomDir(tmp.str().c_str(), fsid.c_str());
    DcmDirectoryRecord& rootRecord = dicomdir->getRootRecord();

    /*
    ** add optional parts to root record
    */
    if (!fsdfid.empty()) {
        dcmInsertString(dicomdir->getDirFileFormat().getDataset(),
                        DCM_FileSetDescriptorFileID, fsdfid);
        dcmInsertString(dicomdir->getDirFileFormat().getDataset(),
                        DCM_SpecificCharacterSetOfFileSetDescriptorFile, scsfsdf);
    }

    iter = goodFileNames.begin();
    while (iter != goodFileNames.end()) {
        if (addToDir(&rootRecord, *iter) == OFFalse) {
            return OFFalse;
        }
        ++iter;
    }


    if (inventAttributes)     // invent missing attributes on all levels
        inventMissingAttributes(&rootRecord);
    else if (inventPatientID) // only invent new PatientID
        inventMissingAttributes(&rootRecord, OFFalse /*recurse*/);

	dicomdir->write();

    return dicomdir;
}

OFBool DicomdirFactory::expandFileNames(OFList<OFString>& fileNames, OFList<OFString>& expandedNames)
{
  OFBool ok = OFTrue;
  HANDLE     hFile;
  WIN32_FIND_DATA stWin32FindData;
  int             ret;

  OFListIterator(OFString) iter = fileNames.begin();
  while (iter != fileNames.end())
  {
    OFString fname(*iter);
    ++iter;

    hFile = FindFirstFile(fname.c_str(), &stWin32FindData);
    if (hFile == INVALID_HANDLE_VALUE)
    {
      CERR << "Error: cannot access: " << fname << endl;
      ok = OFFalse;
    }
    else if(stWin32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      OFList<OFString> subList;
      OFString     newSearchname(fname);
      newSearchname += "\\*";
      FindClose(hFile);

      hFile = FindFirstFile(newSearchname.c_str(), &stWin32FindData);
      ret = (hFile != INVALID_HANDLE_VALUE);

      while(ret)
      {
                if (!cmp(stWin32FindData.cFileName, ".") && !cmp(stWin32FindData.cFileName, ".."))
                {
          OFString subname(fname);
          subname += PATH_SEPARATOR;
          subname += stWin32FindData.cFileName;
          subList.push_back(subname);
                }
        ret = FindNextFile(hFile, &stWin32FindData);
      }

      if(hFile != INVALID_HANDLE_VALUE)
      {
        FindClose(hFile);
        expandFileNames(subList, expandedNames);
          }
    } else {
      expandedNames.push_back(fname);
    }
  }

  return ok;
}