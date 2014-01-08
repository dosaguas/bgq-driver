/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
#define BOOST_TEST_MODULE help
#include <boost/test/unit_test.hpp>

#include "client/options/Parser.h"

#include "test/client/Fixture.h"
#include "test/make_argv.h"

#include <boost/program_options/errors.hpp>

using namespace runjob::client::options;

BOOST_FIXTURE_TEST_SUITE( help, Fixture )

BOOST_AUTO_TEST_CASE( arg )
{
    _argv = make_argv("--help", _argc);
    Parser options(_argc, _argv);
    BOOST_CHECK_EQUAL(
            options.getHelp(),
            true
            );
}

BOOST_AUTO_TEST_CASE( env )
{
    int replace = 1;
    BOOST_CHECK_EQUAL(
            setenv("RUNJOB_HELP", "", replace),
            0
            );
    _argv = make_argv("", _argc);
    Parser options(_argc, _argv);
    BOOST_CHECK_EQUAL(
            options.getHelp(),
            true
            );
    unsetenv("RUNJOB_HELP");
}

BOOST_AUTO_TEST_CASE( arg_short )
{
    _argv = make_argv("-h", _argc);
    Parser options(_argc, _argv);
    BOOST_CHECK_EQUAL(
            options.getHelp(),
            true
            );
}

BOOST_AUTO_TEST_CASE( both_args )
{
    _argv = make_argv("--help -h", _argc);
    BOOST_CHECK_THROW(
            Parser(_argc, _argv),
            boost::program_options::multiple_occurrences
            );
}

BOOST_AUTO_TEST_SUITE_END()

