// SPDX-License-Identifier: GPL-2.0
#include "testmerge.h"
#include "core/device.h"
#include "core/dive.h" // for save_dives()
#include "core/divelog.h"
#include "core/divesite.h"
#include "core/file.h"
#include "core/trip.h"
#include "core/pref.h"
#include <QTextStream>

void TestMerge::initTestCase()
{
	/* we need to manually tell that the resource exists, because we are using it as library. */
	Q_INIT_RESOURCE(subsurface);
	copy_prefs(&default_prefs, &prefs);
}

void TestMerge::cleanup()
{
	clear_dive_file_data();
}

void TestMerge::testMergeEmpty()
{
	/*
	 * check that we correctly merge mixed cylinder dives
	 */
	struct divelog log;
	QCOMPARE(parse_file(SUBSURFACE_TEST_DATA "/dives/test47.xml", &log), 0);
	add_imported_dives(log, IMPORT_MERGE_ALL_TRIPS);
	QCOMPARE(parse_file(SUBSURFACE_TEST_DATA "/dives/test48.xml", &log), 0);
	add_imported_dives(log, IMPORT_MERGE_ALL_TRIPS);
	QCOMPARE(save_dives("./testmerge47+48.ssrf"), 0);
	QFile org(SUBSURFACE_TEST_DATA "/dives/test47+48.xml");
	org.open(QFile::ReadOnly);
	QFile out("./testmerge47+48.ssrf");
	out.open(QFile::ReadOnly);
	QTextStream orgS(&org);
	QTextStream outS(&out);
	QStringList readin = orgS.readAll().split("\n");
	QStringList written = outS.readAll().split("\n");
	while (readin.size() && written.size())
		QCOMPARE(written.takeFirst().trimmed(), readin.takeFirst().trimmed());
}

void TestMerge::testMergeBackwards()
{
	/*
	 * check that we correctly merge mixed cylinder dives
	 */
	struct divelog log;
	QCOMPARE(parse_file(SUBSURFACE_TEST_DATA "/dives/test48.xml", &log), 0);
	add_imported_dives(log, IMPORT_MERGE_ALL_TRIPS);
	QCOMPARE(parse_file(SUBSURFACE_TEST_DATA "/dives/test47.xml", &log), 0);
	add_imported_dives(log, IMPORT_MERGE_ALL_TRIPS);
	QCOMPARE(save_dives("./testmerge47+48.ssrf"), 0);
	QFile org(SUBSURFACE_TEST_DATA "/dives/test48+47.xml");
	org.open(QFile::ReadOnly);
	QFile out("./testmerge47+48.ssrf");
	out.open(QFile::ReadOnly);
	QTextStream orgS(&org);
	QTextStream outS(&out);
	QStringList readin = orgS.readAll().split("\n");
	QStringList written = outS.readAll().split("\n");
	while (readin.size() && written.size())
		QCOMPARE(written.takeFirst().trimmed(), readin.takeFirst().trimmed());
}

QTEST_GUILESS_MAIN(TestMerge)
