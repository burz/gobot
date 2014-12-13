{-# LANGUAGE ScopedTypeVariables #-}

import Control.Applicative ((<*), (*>))
import Control.Exception
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

validateMove :: Position -> Parser Position
validateMove (x, y) = if (x, y) >= (0, 0) && (x, y) <= (19, 19)
    then return (x, y)
    else fail "Illegal move"

move :: Parser Position
move = do
    dataPoints <- many dataPoint
    validateMove $ foldr find (0, 0) dataPoints
    where find (f, a) r = if f == "W"
            then convertMove a
            else if f == "B"
                then convertMove a
                else r

convertScore :: String -> Maybe Double
convertScore (c : _ : s) = if c == 'B'
    then readMaybe s >>= \x -> Just $ -1.0 * x
    else readMaybe s

header :: Parser HeaderInfo
header = do
    dataPoints <- many dataPoint
    let r = foldr find (Nothing, Nothing) dataPoints
    case r of
        (Nothing, _) -> fail "Couldn't read komi"
        (_, Nothing) -> fail "No final score"
        (Just k, Just s) -> return (k, s)
    where find (f, a) (k, s) = if f == "KM"
            then (readMaybe a, s)
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
            r <- parseFile f `catch` \(e :: SomeException) -> do
                putStrLn $ "Win by resignation" ++ show e
                exitFailure
            case r of
                Left e -> do
                    putStrLn $ "Poorly formed file: " ++ show e
                    exitFailure
                Right g -> writeFile o $ show g
        _ -> putStrLn "Usage: reformat filename output"
