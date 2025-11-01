//TestCommands.cpp
// Simple test program to verify command functionality
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include "CommandFactory.h"
#include "Presentation.h"
#include "ISlideRepository.h"
#include "ICommand.h"

class TestRunner {
public:
    bool runAllTests() {
        std::cout << "========================================" << std::endl;
        std::cout << "    Command System Test Suite" << std::endl;
        std::cout << "========================================" << std::endl << std::endl;

        int passed = 0;
        int failed = 0;

        // Test 1: Factory registration
        test("CommandFactory: Registration Check", [&]() {
            return CommandFactory::isRegistered("addslide") &&
                   CommandFactory::isRegistered("removeslide");
        }, passed, failed);

        // Test 2: Add slide command
        test("AddSlideCommand: Basic Execution", [&]() {
            Presentation pres;
            auto cmd = CommandFactory::create("addslide", {});
            cmd->execute(pres);
            return pres.getSlideCount() == 1;
        }, passed, failed);

        // Test 3: Add slide at position
        test("AddSlideCommand: With Position Argument", [&]() {
            Presentation pres;
            auto cmd1 = CommandFactory::create("addslide", {});
            cmd1->execute(pres); // Add first slide
            
            auto cmd2 = CommandFactory::create("addslide", {{"pos", "0"}});
            cmd2->execute(pres); // Add at position 0
            return pres.getSlideCount() == 2;
        }, passed, failed);

        // Test 4: Remove slide command
        test("RemoveSlideCommand: Basic Execution", [&]() {
            Presentation pres;
            auto addCmd = CommandFactory::create("addslide", {});
            addCmd->execute(pres); // Add a slide first
            
            auto removeCmd = CommandFactory::create("removeslide", {});
            removeCmd->execute(pres);
            return pres.getSlideCount() == 0;
        }, passed, failed);

        // Test 5: Remove slide at position
        test("RemoveSlideCommand: With Position Argument", [&]() {
            Presentation pres;
            auto add1 = CommandFactory::create("addslide", {});
            add1->execute(pres);
            auto add2 = CommandFactory::create("addslide", {});
            add2->execute(pres); // Now have 2 slides
            
            auto removeCmd = CommandFactory::create("removeslide", {{"at", "0"}});
            removeCmd->execute(pres);
            return pres.getSlideCount() == 1;
        }, passed, failed);

        // Test 6: Undo functionality
        test("RemoveSlideCommand: Undo Support", [&]() {
            Presentation pres;
            auto addCmd = CommandFactory::create("addslide", {});
            addCmd->execute(pres);
            
            auto removeCmd = CommandFactory::create("removeslide", {});
            removeCmd->execute(pres);
            
            bool beforeUndo = (pres.getSlideCount() == 0);
            removeCmd->undo(pres);
            bool afterUndo = (pres.getSlideCount() == 1);
            
            return beforeUndo && afterUndo;
        }, passed, failed);

        // Test 7: Error handling - invalid command
        test("CommandFactory: Invalid Command Error", [&]() {
            try {
                auto cmd = CommandFactory::create("invalidcommand", {});
                return false; // Should have thrown
            }
            catch (const std::invalid_argument&) {
                return true; // Expected exception
            }
        }, passed, failed);

        // Test 8: Error handling - remove from empty
        test("RemoveSlideCommand: Remove from Empty Error", [&]() {
            Presentation pres;
            auto cmd = CommandFactory::create("removeslide", {});
            try {
                cmd->execute(pres);
                return false; // Should have thrown
            }
            catch (const std::runtime_error&) {
                return true; // Expected exception
            }
        }, passed, failed);

        // Test 9: Error handling - invalid position
        test("RemoveSlideCommand: Invalid Position Error", [&]() {
            Presentation pres;
            auto cmd = CommandFactory::create("removeslide", {{"at", "999"}});
            try {
                cmd->execute(pres);
                return false; // Should have thrown
            }
            catch (const std::out_of_range&) {
                return true; // Expected exception
            }
        }, passed, failed);

        // Test 10: Command name
        test("Commands: GetName() Method", [&]() {
            auto addCmd = CommandFactory::create("addslide", {});
            auto removeCmd = CommandFactory::create("removeslide", {});
            return addCmd->getName() == "AddSlide" && 
                   removeCmd->getName() == "RemoveSlide";
        }, passed, failed);

        // Test 11: CanUndo check
        test("Commands: canUndo() Method", [&]() {
            auto addCmd = CommandFactory::create("addslide", {});
            auto removeCmd = CommandFactory::create("removeslide", {});
            return !addCmd->canUndo() && removeCmd->canUndo();
        }, passed, failed);

        // Test 12: Multiple operations sequence
        test("Integration: Multiple Operations Sequence", [&]() {
            Presentation pres;
            
            // Add 3 slides
            for (int i = 0; i < 3; i++) {
                auto cmd = CommandFactory::create("addslide", {});
                cmd->execute(pres);
            }
            
            // Remove middle slide
            auto removeCmd = CommandFactory::create("removeslide", {{"at", "1"}});
            removeCmd->execute(pres);
            
            // Check result
            bool correct = (pres.getSlideCount() == 2);
            
            // Undo
            removeCmd->undo(pres);
            
            return correct && (pres.getSlideCount() == 3);
        }, passed, failed);

        // Print summary
        std::cout << std::endl << "========================================" << std::endl;
        std::cout << "Test Results: " << passed << " passed, " << failed << " failed" << std::endl;
        std::cout << "========================================" << std::endl;
        
        return failed == 0;
    }

private:
    template<typename Func>
    void test(const std::string& testName, Func testFunc, int& passed, int& failed) {
        std::cout << "[TEST] " << testName << " ... ";
        try {
            if (testFunc()) {
                std::cout << "PASSED" << std::endl;
                passed++;
            }
            else {
                std::cout << "FAILED" << std::endl;
                failed++;
            }
        }
        catch (const std::exception& e) {
            std::cout << "FAILED (Exception: " << e.what() << ")" << std::endl;
            failed++;
        }
    }
};

// Main entry point for tests
// NOTE: This main() is excluded from build to avoid conflict with CLI.cpp
// To run tests, either:
// 1. Create a separate test project and add this file
// 2. Temporarily exclude CLI.cpp from build and include this file
// 3. Uncomment the main() below when you want to run tests

/*
int main() {
    TestRunner runner;
    bool success = runner.runAllTests();
    return success ? 0 : 1;
}
*/

// Alternative: Define a separate function that can be called
// This won't conflict since it's not main()
bool runAllTestsStandalone() {
    TestRunner runner;
    return runner.runAllTests();
}

