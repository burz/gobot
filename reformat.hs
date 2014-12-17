import Control.Applicative ((<*), (*>))
import Data.Char (ord)
import Prelude hiding (catch)
import System.Environment (getArgs)
import System.Exit (exitFailure)
import System.IO
import Text.Parsec
import Text.Parsec.Language
import Text.Parsec.String
import Text.Read (readMaybe)
import qualified Text.Parsec.Token as Token

lexer = Token.makeTokenParser emptyDef
    { Token.identStart = letter
    , Token.identLetter = letter
    }

identifier = Token.identifier lexer
parens     = Token.parens lexer
brackets   = Token.brackets lexer
semi       = Token.semi lexer
semiSep    = Token.semiSep lexer
whiteSpace = Token.whiteSpace lexer

type Position = (Int, Int)
type DataPoint = (String, [String])
type HeaderInfo = (Double, Double, [Position])
data Game = Game Double Double [Position] [Position]

skipParentheses :: Parser ()
skipParentheses = do
    many $ noneOf ['(', ')']
    (char ')' *> return ())
        <|> (char '(' *> skipParentheses *> skipParentheses)
        <|> skipParentheses

dataPoint :: Parser DataPoint
dataPoint = do
    dataType <- identifier
    argument <- many1 . brackets . many $ (try $ char '\\' *> char ']') <|> noneOf [']']
    return $ (dataType, argument)

dataPoints :: Parser [DataPoint]
dataPoints = many $ (try $ (many $ char '(' *> skipParentheses *> whiteSpace) *> dataPoint)
    <|> dataPoint

convertMove :: String -> Position
convertMove (x : y : _) = (ord x - 97, ord y - 97)
convertMove _ = (19, 19)

validateMove :: Position -> Parser Position
validateMove (x, y) = if (x, y) >= (0, 0) && (x, y) <= (19, 19)
    then return (x, y)
    else fail "Illegal move"

move :: Parser Position
move = do
    dps <- dataPoints
    validateMove $ foldr find (0, 0) dps
    where find (f, as) r = if f == "W"
            then case as of (a : _) -> convertMove a
            else if f == "B"
                then case as of (a : _) -> convertMove a
                else r

readScore :: String -> Maybe Double
readScore (c : _ : s) = if c == 'B'
    then readMaybe s >>= \x -> Just $ -1.0 * x
    else readMaybe s

readHandicapPositions :: [String] -> Parser [Position]
readHandicapPositions as = mapM (validateMove . convertMove) as

header :: Parser HeaderInfo
header = do
    dataPoints <- many dataPoint
    let r = foldr find (Nothing, Nothing, Nothing) dataPoints
    case r of
        (Nothing, _, _) -> fail "Couldn't read komi"
        (_, Nothing, _) -> fail "Couldn't read final score"
        (Just k, Just s, Nothing) -> return (k, s, [])
        (Just k, Just s, Just as) -> do
            hp <- readHandicapPositions as
            return (k, s, hp)
    where find (f, as) (k, s, hp) = if f == "KM"
            then case as of (a : _) -> (readMaybe a, s, hp)
            else if f == "RE"
                then case as of (a : _) -> (k, readScore a, hp)
                else if f == "AB"
                    then (k, s, Just as)
                    else (k, s, hp)

game :: Parser Game
game = parens $ semi *> do
    (komi, score, handicapPositions) <- header
    semi
    moves <- semiSep move
    many $ char '(' *> skipParentheses *> whiteSpace
    return $ Game komi score handicapPositions moves

parseFile :: FilePath -> IO (Either ParseError Game)
parseFile f = parseFromFile game f

instance Show Game where
    show (Game komi score handicaps moves) =
        let h = show komi ++ " " ++ show score ++ " " ++ show (length handicaps)
        in let h' = h ++ foldr runMoves "" handicaps
        in h' ++ foldr runMoves "" moves
        where runMoves (x, y) r = "\n" ++ show x ++ " " ++ show y ++ r

main = do
    a <- getArgs
    case a of
        (f : o : _) -> do
            r <- parseFile f
            case r of
                Left e -> do
                    putStrLn $ "Poorly formed file: " ++ show e
                    exitFailure
                Right g -> writeFile o $ show g
        _ -> putStrLn "Usage: reformat filename output"
