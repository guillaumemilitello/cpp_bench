#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <stdexcept>

class Bench
{
public:
    explicit Bench(const std::string& inFileName_,
                   const std::string& outFileName_,
                   const std::string& expFileName_)
        : _inFile(inFileName_)
        , _outFile(outFileName_)
        , _expFile(expFileName_)
        , _expLines(getLines(_expFile))
    {
        std::ofstream ofs;
        ofs.open(outFileName_, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    void start()
    {
        _startClock = std::clock();
    }

    void stop()
    {
        _duration = (std::clock() - _startClock) / (double) 1000;
        _outFile.seekp(0, std::ios::beg);
        _outLines = getLines(_outFile);
    }

    void compareResults()
    {
        static const std::string RESET {"\033[0m"};
        static const std::string BOLDRED {"\033[1m\033[31m"};
        static const std::string BOLDGREEN {"\033[1m\033[32m"};

        bool result {true};
        const size_t outLinesSize {_outLines.size()};
        const size_t expLinesSize {_expLines.size()};
        const size_t lineSpace {std::to_string(std::max(outLinesSize, expLinesSize)).size() + 2};
        for (size_t i=0; i < std::min(outLinesSize, expLinesSize); ++i)
        {
            const auto& outLine{_outLines[i]};
            const auto& expLine{_expLines[i]};
            const auto resOut {computeDiff(outLine, expLine)};
            if (!std::get<bool>(resOut))
            {
                result = false;
                std::cout << toStringSpace(lineSpace, i+1) << std::get<std::string>(resOut) << "\n";
            }
        }

        for (auto i=expLinesSize; i < outLinesSize; ++i)
        {
            result = false;
            const auto resOut {computeDiff(_outLines[i], std::string{})};
            std::cout << toStringSpace(lineSpace, i+1) << std::get<std::string>(resOut) << "\n";
        }

        for (auto i=outLinesSize; i < expLinesSize; ++i)
        {
            result = false;
            const auto resOut {computeDiff(std::string{}, _expLines[i])};
            std::cout << toStringSpace(lineSpace, i+1) << std::get<std::string>(resOut) << "\n";
        }

        if (result)
        {
            std::cout << BOLDGREEN << "PASS [" << _duration << " ms]" << RESET << "\n";
        }
        else
        {
            std::cout << BOLDRED << "FAIL [" << _duration << " ms]" << RESET << "\n";
        }
    }
  
    template<typename T>
    friend Bench& operator>>(Bench& bench_, T& t_)
    {
        bench_._inFile >> t_;
        return bench_;
    }
    
    template<typename T>
    friend Bench& operator<<(Bench& bench_, const T& t_)
    {
        bench_._outFile << t_;
        return bench_;
    }

    template<typename T>
    friend Bench& operator<<(Bench& bench_, T&& t_)
    {
        bench_._outFile << t_;
        return bench_;
    }

private:
    std::tuple<bool, std::string> computeDiff(const std::string& out_, const std::string& exp_) const
    {
        static const std::string RESET {"\033[0m"};
        static const std::string BOLDRED {"\033[1m\033[31m"};
        static const std::string BOLDYELLOW {"\033[1m\033[33m"};
    
        bool result {true};
        std::string output;
        const size_t outSize {out_.size()};
        const size_t expSize {exp_.size()};
        for (size_t i=0; i < std::min(outSize, expSize); ++i)
        {
            if (out_[i] == exp_[i])
            {
                output += out_[i];
            }
            else
            {
                output += BOLDRED + out_[i] + RESET;
                result = false;
            }
        }

        for (auto i=expSize; i < outSize; ++i)
        {
            output += BOLDRED + out_[i] + RESET;
            if (out_[i] != ' ')
            {
                result = false;
            }
        }

        for (auto i=outSize; i < expSize; ++i)
        {
            output += BOLDYELLOW + exp_[i] + RESET;
            if (exp_[i] != ' ')
            {
                result = false;
            }
        }
        return std::make_tuple(result, output);
    }

    std::vector<std::string> getLines(std::fstream& file_)
    {
        std::vector<std::string> lines;
        std::string line;
        while(std::getline(file_, line))
        {
            if (line.back() == '\r')
            {
                line.pop_back();
            }
            lines.emplace_back(line);
        }
        return lines;
    }

    std::string toStringSpace(unsigned space_, unsigned u_)
    {
        std::string uSpace_ {std::to_string(u_)};
        for(unsigned i=uSpace_.size(); i<space_; ++i)
        {
            uSpace_ += " ";
        }
        return uSpace_;
    }

    double          _duration;
    std::clock_t    _startClock;

    std::fstream  _inFile;
    std::fstream  _outFile;
    std::fstream  _expFile;

    const std::vector<std::string> _expLines;
    std::vector<std::string> _outLines;
};
