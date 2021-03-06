#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# Copyright 2017, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(DATA61_BSD)
#

from __future__ import absolute_import, division, print_function, \
    unicode_literals

import os, six, sys, unittest

ME = os.path.abspath(__file__)

# Make CAmkES importable
sys.path.append(os.path.join(os.path.dirname(ME), '../../..'))

from camkes.internal.tests.utils import CAmkESTest
from camkes.parser import ParseError

class TestObjects(CAmkESTest):
    def test_basic_parse_error(self):
        with self.assertRaises(ParseError):
            raise ParseError('hello world')

    def test_parse_error_message(self):
        with six.assertRaisesRegex(self, ParseError, '.*hello world.*'):
            raise ParseError('hello world')

    def test_parse_error_filename(self):
        with six.assertRaisesRegex(self, ParseError, '^myfile:.*hello world.*'):
            raise ParseError('hello world', filename='myfile')

    def test_parse_error_lineno(self):
        with six.assertRaisesRegex(self, ParseError, '.*10:.*hello world.*'):
            raise ParseError('hello world', lineno=10)

    def test_parse_error_filename_lineno(self):
        with six.assertRaisesRegex(self, ParseError, '^myfile:.*10:.*hello world.*'):
            raise ParseError('hello world', filename='myfile', lineno=10)

if __name__ == '__main__':
    unittest.main()
