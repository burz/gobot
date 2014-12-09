import Text.Parsec
import qualified Text.Parsec.Token as Token
import Text.Parsec.Language
import Text.Parsec.String
import Control.Applicative ((<*), (*>))
import Data.Char (ord)
import System.Environment (getArgs)
import System.IO

lexer = Token.makeTokenParser emptyDef
    { Token.identStart = letter
    , Token.identLetter = letter
    }

identifier = Token.identifier lexer
parens     = Token.parens lexer
brackets   = Token.brackets lexer
semi       = Token.semi lexer
semiSep    = Token.semiSep lexer

type Position = (Int, Int)
type DataPoint = (String, String)
type HeaderInfo = (Double, Double)
data Game = Game Double Double [Position]

dataPoint :: Parser DataPoint
dataPoint = do
    dataType <- identifier
    argument <- brackets . many $ noneOf [']']
    return $ (dataType, argument)

convertMove :: String -> Position
convertMove (x : y : _) = (ord x - 97, ord y - 97)

move :: Parser Position
move = do
    dataPoints <- many dataPoint
    return $ foldr find (0, 0) dataPoints
    where find (f, a) r = if f == "W"
            then convertMove a
            else if f == "B"
                then convertMove a
                else r

convertScore :: String -> Double
convertScore (c : _ : s) = if c == 'B'
    then -1.0 * read s
    else read s

header :: Parser HeaderInfo
header = do
    dataPoints <- many dataPoint
    return $ foldr find (0.0, 0.0) dataPoints
    where find (f, a) (k, s) = if f == "KM"
            then (read a, s)
            else if f == "RE"
                then (k, convertScore a)
                else (k, s)
    
game :: Parser Game
game = parens $ semi *> do
    (komi, score) <- header
    semi
    moves <- semiSep move
    return $ Game komi score moves

parseFile :: FilePath -> IO (Either ParseError Game)
parseFile f = parseFromFile (game <* eof) f

instance Show Game where
    show (Game komi score moves) =
        let h = show komi ++ " " ++ show score
        in h ++ foldr runMoves "" moves
        where runMoves (x, y) r = "\n" ++ show x ++ " " ++ show y ++ r

main = do
    a <- getArgs
    case a of
        (f : o : _) -> do
            r <- parseFile f
            case r of
                Left e -> putStrLn $ "Poorly formed file: " ++ show e
                Right g -> do
                    writeFile o $ show g
        _ -> putStrLn "Usage: reformat filename output"
